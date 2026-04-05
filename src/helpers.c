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

#include <stdio.h>       // snprintf
#include "pico/stdlib.h" // sleep
#include "error.h"
#include "helpers.h"
#include "config.h"
#include "usb.h"
#include "epaper.h"
#include "gui.h"

#ifdef FORMAT_DATA_INCLUDE_IB
#define FORMAT_DATA_IB "iB"
#else
#define FORMAT_DATA_IB ""
#endif

/**
 * \brief Format a data size in bytes, into a human readable string with units, e.g. "1.2M"
 * \param output Buffer to write the string into
 * \param input Data size in bytes
 * \param length Length of the output buffer
 * \return Pointer to the output buffer
 */
char *format_data_size(char *output, uint32_t input, uint32_t length)
{
  if (input < 1024)
  {
    snprintf(output, length, "%uB", (unsigned int)input);
    return output;
  }
  else if (input < 1024 * 1024)
  {
    uint32_t whole = input / 1024;
    uint32_t tenths = ((input % 1024) * 10 + 512) / 1024;
    if (tenths >= 10)
    {
      whole++;
      tenths -= 10;
    }
    snprintf(output, length, "%lu.%luK" FORMAT_DATA_IB, (unsigned long)whole, (unsigned long)tenths);
    return output;
  }
  else
  {
    uint32_t whole = input / (1024 * 1024);
    uint32_t tenths = ((input % (1024 * 1024)) * 10 + 524288) / (1024 * 1024);
    if (tenths >= 10)
    {
      whole++;
      tenths -= 10;
    }
    snprintf(output, length, "%lu.%luM" FORMAT_DATA_IB, (unsigned long)whole, (unsigned long)tenths);
    return output;
  }
}

/**
 * \brief Try to mount the filesystem
 * \param fail_gracefully Whether to continue on error (true) or halt (false)
 * \return True if the drive was successfully mounted, false otherwise
 */
bool attempt_mount_flash(FATFS *fatfs_work_area, bool fail_gracefully)
{
  FRESULT fatfs_result = f_mount(fatfs_work_area, "", 0);
  if (fatfs_result == FR_NO_FILESYSTEM)
    return false;
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_MOUNT, true);
    while (1)
    {
    }
  }

  fatfs_result = f_setlabel("mReader");
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_SETLABEL, false);
  }

  DIR root_directory;
  fatfs_result = f_opendir(&root_directory, "/");
  if (fatfs_result == FR_NO_FILESYSTEM)
    return false;
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_OPENDIR, true);
    return false;
  }

  FILINFO file;
  fatfs_result = f_readdir(&root_directory, &file);
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_READDIR, fail_gracefully);
    return false;
  }

  fatfs_result = f_closedir(&root_directory);
  if (fatfs_result != FR_OK)
    error(ERROR_FATFS_CLOSEDIR, fail_gracefully);

  return true;
}

bool read_file_at_offset(FIL *file, uint32_t offset, char *buffer, uint32_t buffer_size)
{
  FRESULT fatfs_result = f_lseek(file, offset);
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_LSEEK, false);
    return false;
  }

  fatfs_result = f_read(file, buffer, buffer_size, NULL);
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
    sleep_ms(1000);
    error(ERROR_FATFS_LSEEK, false);
    return false;
  }

  return true;
}
