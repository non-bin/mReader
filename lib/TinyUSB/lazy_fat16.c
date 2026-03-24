#include "msc_disk.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static void write_sector(uint32_t lba, const void *buf512)
{
  msc_disk_write_block(lba, buf512);
}

static void zero_sector(uint32_t lba)
{
  uint8_t z[DISK_BLOCK_SIZE];
  memset(z, 0, sizeof(z));
  write_sector(lba, z);
}

// Little-endian helpers
static void le16(uint8_t *p, uint16_t v)
{
  p[0] = (uint8_t)v;
  p[1] = (uint8_t)(v >> 8);
}
static void le32(uint8_t *p, uint32_t v)
{
  p[0] = (uint8_t)v;
  p[1] = (uint8_t)(v >> 8);
  p[2] = (uint8_t)(v >> 16);
  p[3] = (uint8_t)(v >> 24);
}

// Very small FAT16 formatter for 1 MiB disk
bool msc_disk_lazy_format_fat16(void)
{
  if (msc_disk_is_formatted())
    return true;

  const uint16_t bytes_per_sector = 512;
  const uint8_t sectors_per_cluster = 1;

  const uint16_t reserved_sectors = 1;
  const uint8_t num_fats = 2;
  const uint16_t root_entries = 512; // 512 entries => 32 sectors
  const uint16_t root_dir_sectors = (uint16_t)((root_entries * 32u + (bytes_per_sector - 1u)) / bytes_per_sector);

  const uint32_t total_sectors = (uint32_t)(DISK_SIZE_BYTES / bytes_per_sector); // 2048 for 1 MiB

  // Choose a FAT size that fits.
  // We’ll iterate to find a stable fat_sectors count.
  uint16_t fat_sectors = 1;
  while (1)
  {
    uint32_t data_sectors = total_sectors - reserved_sectors - (uint32_t)num_fats * fat_sectors - root_dir_sectors;
    uint32_t cluster_count = data_sectors / sectors_per_cluster;

    // FAT16 if cluster_count < 65525
    // FAT entries: cluster_count + 2 reserved. Each FAT16 entry is 2 bytes.
    uint32_t fat_bytes_needed = (cluster_count + 2u) * 2u;
    uint16_t fat_sectors_needed = (uint16_t)((fat_bytes_needed + (bytes_per_sector - 1u)) / bytes_per_sector);

    if (fat_sectors_needed == fat_sectors)
      break;
    fat_sectors = fat_sectors_needed;
    if (fat_sectors > 128)
      return false; // sanity
  }

  uint32_t first_fat_lba = reserved_sectors;
  uint32_t second_fat_lba = first_fat_lba + fat_sectors;
  uint32_t root_lba = second_fat_lba + fat_sectors;
  uint32_t data_lba = root_lba + root_dir_sectors;

  // --- Boot sector (BPB + EBPB) ---
  uint8_t bs[512];
  memset(bs, 0, sizeof(bs));

  bs[0] = 0xEB;
  bs[1] = 0x3C;
  bs[2] = 0x90;                  // JMP
  memcpy(&bs[3], "MSDOS5.0", 8); // OEM
  le16(&bs[11], bytes_per_sector);
  bs[13] = sectors_per_cluster;
  le16(&bs[14], reserved_sectors);
  bs[16] = num_fats;
  le16(&bs[17], root_entries);

  // Total sectors (16-bit) fits (2048)
  le16(&bs[19], (uint16_t)total_sectors);

  bs[21] = 0xF8;              // media descriptor (fixed disk)
  le16(&bs[22], fat_sectors); // sectors per FAT (FAT16)
  le16(&bs[24], 1);           // sectors per track (dummy)
  le16(&bs[26], 1);           // number of heads (dummy)
  le32(&bs[28], 0);           // hidden sectors

  // EBPB
  bs[36] = 0x80;                  // drive number
  bs[38] = 0x29;                  // boot sig
  le32(&bs[39], 0x66642069);      // volume serial
  memcpy(&bs[43], "MREADER", 7);  // volume label (11 chars)
  memcpy(&bs[54], "FAT16   ", 8); // fs type string

  bs[510] = 0x55;
  bs[511] = 0xAA;

  write_sector(0, bs);

  // --- Zero FAT + root dir area ---
  for (uint32_t i = 0; i < (uint32_t)fat_sectors * num_fats; i++)
  {
    zero_sector(first_fat_lba + i);
  }

  // Create Volume Label entry in the first root directory sector
  uint8_t root_sec[512];
  memset(root_sec, 0, sizeof(root_sec));
  memcpy(&root_sec[0], "MREADER", 7);
  root_sec[11] = 0x08; // ATTR_VOLUME_ID
  write_sector(root_lba, root_sec);

  for (uint32_t i = 1; i < root_dir_sectors; i++)
  {
    zero_sector(root_lba + i);
  }

  // --- FAT[0] and FAT[1] initial entries ---
  // FAT16 entry 0: media descriptor in low byte, rest 0xFF
  // FAT16 entry 1: end-of-chain
  uint8_t fat0[512];
  memset(fat0, 0, sizeof(fat0));
  fat0[0] = 0xF8;
  fat0[1] = 0xFF;
  fat0[2] = 0xFF;
  fat0[3] = 0xFF;

  write_sector(first_fat_lba, fat0);
  write_sector(second_fat_lba, fat0);

  // Optionally: zero first data cluster area not required.

  (void)data_lba;
  return true;
}

static uint16_t le16_read(const uint8_t *p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t le32_read(const uint8_t *p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

int msc_disk_get_file_list(msc_file_info_t *files, int max_files) {
    if (!msc_disk_is_formatted()) return 0;

    uint8_t bs[512];
    msc_disk_read_block(0, bs);

    uint16_t bytes_per_sector = le16_read(&bs[11]);
    uint8_t sectors_per_cluster = bs[13];
    uint16_t reserved_sectors = le16_read(&bs[14]);
    uint8_t num_fats = bs[16];
    uint16_t root_entries = le16_read(&bs[17]);
    uint16_t fat_sectors = le16_read(&bs[22]);

    uint32_t root_dir_sectors = (root_entries * 32u + (bytes_per_sector - 1u)) / bytes_per_sector;
    uint32_t root_lba = reserved_sectors + (num_fats * fat_sectors);

    int count = 0;
    for (uint32_t sec = 0; sec < root_dir_sectors; sec++) {
        uint8_t buf[512];
        msc_disk_read_block(root_lba + sec, buf);

        for (int i = 0; i < 512; i += 32) {
            uint8_t attr = buf[i + 11];
            uint8_t first_char = buf[i];

            if (first_char == 0x00) break; // End of directory
            if (first_char == 0xE5) continue; // Deleted file
            if (attr == 0x0F) continue; // LFN entry
            if (attr & 0x08) continue; // Volume label or Directory

            // It's a valid file!
            if (count < max_files) {
                for (int j = 0; j < 8; j++) {
                    files[count].name[j] = buf[i + j];
                }
                files[count].name[8] = 0;
                // Trim right
                for (int j = 7; j >= 0 && files[count].name[j] == ' '; j--) {
                    files[count].name[j] = 0;
                }

                for (int j = 0; j < 3; j++) {
                    files[count].ext[j] = buf[i + 8 + j];
                }
                files[count].ext[3] = 0;
                for (int j = 2; j >= 0 && files[count].ext[j] == ' '; j--) {
                    files[count].ext[j] = 0;
                }

                files[count].start_cluster = le16_read(&buf[i + 26]);
                files[count].size = le32_read(&buf[i + 28]);
                
                count++;
            }
        }
    }
    return count;
}

int msc_disk_read_file(uint16_t start_cluster, uint32_t file_size, uint8_t *buffer, uint32_t buffer_size) {
    if (!msc_disk_is_formatted() || start_cluster < 2 || file_size == 0) return 0;
    
    uint8_t bs[512];
    msc_disk_read_block(0, bs);

    uint16_t bytes_per_sector = le16_read(&bs[11]);
    uint8_t sectors_per_cluster = bs[13];
    uint16_t reserved_sectors = le16_read(&bs[14]);
    uint8_t num_fats = bs[16];
    uint16_t root_entries = le16_read(&bs[17]);
    uint16_t fat_sectors = le16_read(&bs[22]);

    uint32_t root_dir_sectors = (root_entries * 32u + (bytes_per_sector - 1u)) / bytes_per_sector;
    uint32_t first_fat_lba = reserved_sectors;
    uint32_t data_lba = reserved_sectors + (num_fats * fat_sectors) + root_dir_sectors;

    uint32_t cluster_size = bytes_per_sector * sectors_per_cluster;
    uint16_t current_cluster = start_cluster;
    uint32_t bytes_read = 0;

    uint8_t fat_sec[512];
    uint32_t current_fat_sec = 0xFFFFFFFF;

    while (current_cluster >= 2 && current_cluster <= 0xFFF6 && bytes_read < file_size && bytes_read < buffer_size) {
        // Read data cluster
        uint32_t cluster_lba = data_lba + (current_cluster - 2) * sectors_per_cluster;
        
        for (uint8_t sec = 0; sec < sectors_per_cluster; sec++) {
            uint8_t data_sec[512];
            msc_disk_read_block(cluster_lba + sec, data_sec);
            
            uint32_t copy_size = 512;
            if (file_size - bytes_read < copy_size) copy_size = file_size - bytes_read;
            if (buffer_size - bytes_read < copy_size) copy_size = buffer_size - bytes_read;
            
            memcpy(buffer + bytes_read, data_sec, copy_size);
            bytes_read += copy_size;
            if (bytes_read >= file_size || bytes_read >= buffer_size) break;
        }

        // Get next cluster from FAT
        uint32_t fat_offset = current_cluster * 2;
        uint32_t fat_sec_index = first_fat_lba + (fat_offset / 512);
        
        if (current_fat_sec != fat_sec_index) {
            msc_disk_read_block(fat_sec_index, fat_sec);
            current_fat_sec = fat_sec_index;
        }
        
        current_cluster = le16_read(&fat_sec[fat_offset % 512]);
    }

    return bytes_read;
}
