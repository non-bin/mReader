#pragma once
#include <stdbool.h>
#include <stdint.h>

#define DISK_BLOCK_SIZE 512u
#define DISK_SIZE_BYTES (1u * 1024u * 1024u)
#define DISK_BLOCK_COUNT (DISK_SIZE_BYTES / DISK_BLOCK_SIZE)

#define FLASH_TOTAL_BYTES (2u * 1024u * 1024u) // RP2040-Zero stated 2 MiB flash
#define DISK_FLASH_OFFSET (FLASH_TOTAL_BYTES - DISK_SIZE_BYTES)

bool msc_disk_lazy_format_fat16(void);
bool msc_disk_is_formatted(void);
void msc_disk_read_block(uint32_t lba, void *dst512);
void msc_disk_write_block(uint32_t lba, const void *src512);

typedef struct
{
  char name[9];           // 8 chars + null terminator
  char ext[4];            // 3 chars + null terminator
  uint32_t size;          // file size in bytes
  uint16_t start_cluster; // starting cluster number (2-based)
} msc_file_info_t;

int msc_disk_get_file_list(msc_file_info_t *files, int max_files);
int msc_disk_read_file(uint16_t start_cluster, uint32_t size, uint8_t *buffer, uint32_t buffer_size);
