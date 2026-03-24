#include "tusb.h"
#include "msc_disk.h"
#include <string.h>

bool tud_msc_is_writable_cb(uint8_t lun)
{
  (void)lun;
  return true;
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
  (void)lun;
  return true; // Flash disk is always ready
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
  (void)lun;
  memcpy(vendor_id, "mReader  ", 8);
  memcpy(product_id, "Flash Disk       ", 16);
  memcpy(product_rev, "1.0 ", 4);
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size)
{
  (void)lun;
  *block_count = DISK_BLOCK_COUNT;
  *block_size = DISK_BLOCK_SIZE;
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
  (void)lun;
  (void)power_condition;
  (void)start;
  (void)load_eject;
  return true;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
  (void)lun;

  uint8_t tmp[DISK_BLOCK_SIZE];
  uint32_t done = 0;

  while (done < bufsize)
  {
    uint32_t cur_lba = lba + (done / DISK_BLOCK_SIZE);
    uint32_t cur_off = (done == 0) ? offset : 0;
    uint32_t n = DISK_BLOCK_SIZE - cur_off;
    if (n > (bufsize - done))
      n = bufsize - done;

    msc_disk_read_block(cur_lba, tmp);
    memcpy((uint8_t *)buffer + done, tmp + cur_off, n);
    done += n;
  }

  return (int32_t)bufsize;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
  (void)lun;

  // Read/modify/write 512-byte blocks to handle partial writes (offset != 0).
  uint8_t tmp[DISK_BLOCK_SIZE];
  uint32_t done = 0;

  while (done < bufsize)
  {
    uint32_t cur_lba = lba + (done / DISK_BLOCK_SIZE);
    uint32_t cur_off = (done == 0) ? offset : 0;
    uint32_t n = DISK_BLOCK_SIZE - cur_off;
    if (n > (bufsize - done))
      n = bufsize - done;

    msc_disk_read_block(cur_lba, tmp);
    memcpy(tmp + cur_off, buffer + done, n);
    msc_disk_write_block(cur_lba, tmp);

    done += n;
  }

  return (int32_t)bufsize;
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize)
{
  (void)buffer;
  (void)bufsize;

  // For simplicity, let TinyUSB handle what it can. Unknown commands => ILLEGAL REQUEST
  tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
  return -1;
}
