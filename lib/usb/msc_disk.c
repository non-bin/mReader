/**
 * mReader is a micro e-reader
 * Copyright (C) 2026  Alice Jacka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#include "tusb.h"
#include "flash.h"

/**
 * \brief Identifies the USB drive to the host
 */
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
  memcpy(vendor_id, "AliceJac", 8);
  memcpy(product_id, "mReader          ", 16);
  memcpy(product_rev, "0.1 ", 4);
}

/**
 * \brief Returns the number of blocks and block size of the USB drive
 */
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size)
{
  *block_count = DISK_BLOCK_COUNT;
  *block_size = DISK_BLOCK_SIZE;
}

/**
 * \return Whether the drive is ready to be accessed
 *
 */
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
  return true;
}

/**
 * \brief Run when the host ejects the drive or powers it down
 * \return Whether to allow the operation
 */
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
  if (load_eject)
  {
    flash_sync();
  }
  return true;
}

/**
 * \brief Read data from the drive
 * \return The number of bytes read, or -1 on error
 */
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t sector, uint32_t byte_offset_into_sector, void *buffer, uint32_t length_bytes)
{
  uint8_t sector_buffer[DISK_BLOCK_SIZE];
  uint32_t bytes_read = 0;

  while (bytes_read < length_bytes)
  {
    uint32_t total_offset = byte_offset_into_sector + bytes_read;
    uint32_t current_sector = sector + (total_offset / DISK_BLOCK_SIZE);
    uint32_t current_offset = total_offset % DISK_BLOCK_SIZE;

    uint32_t bytes_to_copy = DISK_BLOCK_SIZE - current_offset;
    if (bytes_to_copy > (length_bytes - bytes_read))
      bytes_to_copy = length_bytes - bytes_read;

    if (bytes_to_copy == DISK_BLOCK_SIZE)
    {
      flash_read((uint8_t *)buffer + bytes_read, current_sector, 1);
    }
    else
    {
      flash_read(sector_buffer, current_sector, 1);
      memcpy((uint8_t *)buffer + bytes_read, sector_buffer + current_offset, bytes_to_copy);
    }

    bytes_read += bytes_to_copy;
  }

  return (int32_t)length_bytes;
}

/**
 * \brief Write data to the drive
 * \return The number of bytes written, or -1 on error
 */
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t sector, uint32_t byte_offset_into_sector, uint8_t *buffer, uint32_t length_bytes)
{
  uint8_t sector_buffer[DISK_BLOCK_SIZE];
  uint32_t bytes_written = 0;

  while (bytes_written < length_bytes)
  {
    uint32_t total_offset = byte_offset_into_sector + bytes_written;
    uint32_t current_sector = sector + (total_offset / DISK_BLOCK_SIZE);
    uint32_t current_offset = total_offset % DISK_BLOCK_SIZE;

    uint32_t bytes_to_update = DISK_BLOCK_SIZE - current_offset;
    if (bytes_to_update > (length_bytes - bytes_written))
      bytes_to_update = length_bytes - bytes_written;

    if (bytes_to_update == DISK_BLOCK_SIZE)
    {
      flash_write((uint8_t *)buffer + bytes_written, current_sector);
    }
    else
    {
      flash_read(sector_buffer, current_sector, 1);
      memcpy(sector_buffer + current_offset, (uint8_t *)buffer + bytes_written, bytes_to_update);
      flash_write(sector_buffer, current_sector);
    }

    bytes_written += bytes_to_update;
  }

  return (int32_t)length_bytes;
}

/**
 * \brief Run when the host finishes writing data to the drive
 */
void tud_msc_write10_complete_cb(uint8_t lun)
{
  flash_sync();
}

/**
 * \brief Run when the host sends a SCSI command
 * \return 0 if the command was handled successfully, -1 on error
 */
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t length_bytes)
{
  switch (scsi_cmd[0])
  {
  case 0x35: // SCSI SYNCHRONIZE CACHE (10)
    flash_sync();
    return 0;

  default:
    tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
    return -1;
  }
}
