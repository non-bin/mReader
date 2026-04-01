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

#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdbool.h>
#include <stdint.h>

// Codes are flashed MSB to LSB, red is 0 yellow is 1
typedef enum
{
  ERROR_NONE = 0x00,
  ERROR_UNKNOWN = 0x01,

  ERROR_EPAPER_BUSY = 0x10, // TODO The display has been busy for longer than EPAPER_TIMEOUT_MS

  ERROR_FIRMWARE_TOO_LONG = 0x20, // TODO Add a check to the linker script

  ERROR_GUI_INVALID_ROTATION = 0x30,
  ERROR_GUI_INVALID_MIRROR = 0x31,
  ERROR_GUI_INVALID_BITS_PER_PIXEL = 0x32,
  ERROR_GUI_DRAW_OUT_OF_BOUNDS = 0x33,

  ERROR_FATFS_MKFS = 0x40,
  ERROR_FATFS_SETLABEL = 0x41,
  ERROR_FATFS_MOUNT = 0x42,
  ERROR_FATFS_OPENDIR = 0x43,
  ERROR_FATFS_READDIR = 0x44,
  ERROR_FATFS_CLOSEDIR = 0x45,
  ERROR_FATFS_OPEN = 0x46,
  ERROR_FATFS_LSEEK = 0x47,
} error_t;

void flash_code(error_t error, uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1);
void error(error_t error, bool halt);

#endif /* __ERROR_H_ */
