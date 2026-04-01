/**
 * \brief Low level disk interface module
 *
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
 * \copyright 2025 by ChaN <https://elm-chan.org/fsw/ff/>
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#include <stdint.h>
#include <string.h> // memcpy
#include "ff.h"
#include "diskio.h"
#include "flash.h"
#include "tinyusb.h" // usb_mounted
#include "hardware/flash.h"
#include "hardware/sync.h" // interrupts
#include "error.h"

static inline uint32_t lba_to_flash_offset(uint32_t lba)
{
  return DISK_FLASH_OFFSET + lba * DISK_BLOCK_SIZE;
}

DSTATUS disk_status(BYTE drive_number)
{
  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
    if (!usb_mounted)
      return STA_OK;
  }
  return STA_NOINIT;
}

DSTATUS disk_initialize(BYTE drive_number)
{
  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    extern uint8_t __flash_binary_end;
    uintptr_t fw_end_off = (uintptr_t)&__flash_binary_end - (uintptr_t)XIP_BASE;
    if (fw_end_off > FLASH_FIRMWARE_BYTES)
    {
      error(ERROR_FIRMWARE_TOO_LONG, true);
      while (1)
      {
      }
    }

    return STA_OK;
  }
  }
  return STA_NOINIT;
}

DRESULT disk_read(
    BYTE drive_number,
    BYTE *buff,   /* Data buffer to store read data */
    LBA_t sector, /* Start sector in LBA */
    UINT count    /* Number of sectors to read */
)
{
  if (disk_status(drive_number) & STA_NOINIT)
    return RES_NOTRDY;

  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    uint32_t offset = lba_to_flash_offset(sector);
    do
    {
      memcpy(buff, (const void *)(XIP_BASE + offset), DISK_BLOCK_SIZE);
      buff += DISK_BLOCK_SIZE;
    } while (--count);

    return count ? RES_ERROR : RES_OK;
  }
  }

  return RES_PARERR;
}

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE drive_number,
    const BYTE *buff, /* Data to be written */
    LBA_t sector,     /* Start sector in LBA */
    UINT count        /* Number of sectors to write */
)
{
  if (disk_status(drive_number) & STA_NOINIT)
    return RES_NOTRDY;

  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    uint32_t offset = lba_to_flash_offset(sector);

    // TODO write cache, and impliment CTRL_SYNC https://elm-chan.org/fsw/ff/doc/dioctl.html
    // Write one 512-byte block using a 4KB read-modify-erase-program cycle
    uint32_t sector_base_address = offset & ~(FLASH_SECTOR_SIZE - 1u);
    uint32_t offset_in_sector = offset - sector_base_address;

    uint8_t sector_buffer[FLASH_SECTOR_SIZE];
    memcpy(sector_buffer, (const void *)(XIP_BASE + sector_base_address), FLASH_SECTOR_SIZE);
    memcpy(&sector_buffer[offset_in_sector], buff, DISK_BLOCK_SIZE);

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(sector_base_address, FLASH_SECTOR_SIZE);
    flash_range_program(sector_base_address, sector_buffer, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);

    return RES_OK;
  }
  }

  return RES_PARERR;
}

#endif

DRESULT disk_ioctl(
    BYTE drive_number, /* Physical drive nmuber (0..) */
    BYTE command,      /* Control code */
    void *buff         /* Buffer to send/receive control data */
)
{
  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
    switch (command)
    {
    case CTRL_SYNC:
      return RES_OK;
    case GET_SECTOR_COUNT:
      *(LBA_t *)buff = DISK_BLOCK_COUNT;
    case GET_SECTOR_SIZE:
      *(LBA_t *)buff = DISK_BLOCK_SIZE;
    case GET_BLOCK_SIZE:
      *(LBA_t *)buff = FLASH_BLOCK_SIZE;
    case CTRL_TRIM:
      return RES_OK;
    }

    break;
  }

  return RES_PARERR;
}
