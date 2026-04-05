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

#ifndef __FLASH_H_
#define __FLASH_H_

#include "config.h"

#define DISK_FLASH_OFFSET (FLASH_TOTAL_BYTES - DISK_SIZE_BYTES)
#define DISK_BLOCK_COUNT (DISK_SIZE_BYTES / DISK_BLOCK_SIZE)

void flash_init();
void flash_sync();
uint32_t flash_read(uint8_t *output_buffer, const uint32_t disk_block, const uint32_t disk_blocks_to_read);
void flash_write(const uint8_t *input_buffer, const uint32_t disk_block);

#endif /* __FLASH_H_ */
