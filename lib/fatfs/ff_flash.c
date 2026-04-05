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
#include <string.h>         // memcpy
#include "hardware/flash.h" // FLASH_SECTOR_SIZE
#include "ff.h"
#include "diskio.h"
#include "usb.h" // usb_mounted
#include "flash.h"
#include "error.h"

#define STA_OK 0x00 // DSTATUS

/**
 * \brief Get the status of a drive
 */
DSTATUS disk_status(BYTE drive_number)
{
  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
    // if (!usb_mounted) // TODO
    return STA_OK;
  }
  return STA_NOINIT;
}

/**
 * \brief Initialise a drive
 */
DSTATUS disk_initialize(BYTE drive_number)
{
  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    flash_init();

    return STA_OK;
  }
  }
  return STA_NOINIT;
}

/**
 * \brief Read blocks from a drive
 */
DRESULT disk_read(
    BYTE drive_number,
    BYTE *buff,       /* Data buffer to store read data */
    LBA_t disk_block, /* Start block in LBA */
    UINT count        /* Number of blocks to read */
)
{
  if (disk_status(drive_number) & STA_NOINIT)
    return RES_NOTRDY;

  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    flash_read(buff, disk_block, count);

    return RES_OK;

    // return RES_ERROR;
  }
  }

  return RES_PARERR;
}

#if FF_FS_READONLY == 0

/**
 * \brief Write blocks to a drive
 */
DRESULT disk_write(
    BYTE drive_number,
    const BYTE *buff, /* Data to be written */
    LBA_t disk_block, /* Start block in LBA */
    UINT count        /* Number of blocks to write */
)
{
  if (disk_status(drive_number) & STA_NOINIT)
    return RES_NOTRDY;

  switch (drive_number)
  {
  case FF_DRIVE_FLASH:
  {
    do
    {
      flash_write(buff, disk_block);
      disk_block++;
      buff += DISK_BLOCK_SIZE;
    } while (--count);

    return RES_OK;
  }
  }

  return RES_PARERR;
}

#endif

/**
 * \brief Run control commands on a drive
 */
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
    case CTRL_SYNC: // Flush write cache, if any
      flash_sync();
      return RES_OK;

    case GET_SECTOR_COUNT: // Get number of disk blocks
      *(LBA_t *)buff = DISK_BLOCK_COUNT;
      return RES_OK;

    case GET_SECTOR_SIZE: // Get disk block size, in bytes
      *(LBA_t *)buff = DISK_BLOCK_SIZE;
      return RES_OK;

    case GET_BLOCK_SIZE: // Get flash sector size, in units of disk blocks
      *(LBA_t *)buff = (FLASH_SECTOR_SIZE / DISK_BLOCK_SIZE);
      return RES_OK;

    case CTRL_TRIM:
      return RES_OK;
    }

    break;
  }

  return RES_PARERR;
}
