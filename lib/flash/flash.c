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
#include <stdlib.h> // malloc, free
#include "hardware/flash.h"
#include "hardware/sync.h" // interrupts
#include "error.h"
#include "flash.h"
#include "pico/stdlib.h" // time

#define DISK_SIZE_IN_FLASH_SECTORS (DISK_SIZE_BYTES % FLASH_SECTOR_SIZE == 0 ? (DISK_SIZE_BYTES / FLASH_SECTOR_SIZE) : (DISK_SIZE_BYTES / FLASH_SECTOR_SIZE) + 1)

static uint16_t currently_allocated_sectors = 0; // Track how many flash sectors we have allocated cache for

static struct
{
  bool dirty;
  uint8_t *cache_pointer;
} write_cache_sector_info[DISK_SIZE_IN_FLASH_SECTORS] = {false, NULL};

/**
 * \brief Convert a disk block number to an address in flash memory
 */
static inline uint32_t disk_block_to_flash_address(const uint32_t disk_block)
{
  return DISK_FLASH_OFFSET + disk_block * DISK_BLOCK_SIZE;
}

/**
 * \brief Initialise flash subsystem, checking firmware size fits and any other necessary setup
 * TODO Remove from here and add this to the link script
 */
void flash_init()
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
}

/**
 * \brief Synchronize the write cache to flash
 */
void flash_sync()
{
  for (uint16_t flash_sector = 0; flash_sector < DISK_SIZE_IN_FLASH_SECTORS; flash_sector++)
  {
    if (write_cache_sector_info[flash_sector].cache_pointer != NULL)
    {
      if (write_cache_sector_info[flash_sector].dirty)
      {
        uint32_t flash_address = disk_block_to_flash_address(flash_sector * (FLASH_SECTOR_SIZE / DISK_BLOCK_SIZE));
        uint32_t flash_sector_base = flash_address & ~(FLASH_SECTOR_SIZE - 1u);

        uint32_t prior_interrupt_flags = save_and_disable_interrupts();
        flash_range_erase(flash_sector_base, FLASH_SECTOR_SIZE);
        flash_range_program(flash_sector_base, write_cache_sector_info[flash_sector].cache_pointer, FLASH_SECTOR_SIZE);
        restore_interrupts(prior_interrupt_flags);
      }

      free(write_cache_sector_info[flash_sector].cache_pointer);
      write_cache_sector_info[flash_sector].cache_pointer = NULL;
      write_cache_sector_info[flash_sector].dirty = false;
    }
  }
  currently_allocated_sectors = 0;
}

/**
 * \brief Read from flash memory and the write cache
 * \param output_buffer Buffer to read data into
 * \param disk_block Starting disk block number to read from
 * \param disk_blocks_to_read Number of disk blocks to read
 * \return The number of blocks read
 */
uint32_t flash_read(uint8_t *output_buffer, const uint32_t disk_block, const uint32_t disk_blocks_to_read)
{
  for (uint32_t i = 0; i < disk_blocks_to_read; i++)
  {
    uint32_t current_disk_block = disk_block + i;
    uint32_t flash_sector = (current_disk_block * DISK_BLOCK_SIZE) / FLASH_SECTOR_SIZE;

    if (write_cache_sector_info[flash_sector].cache_pointer != NULL)
    {
      uint32_t block_offset_within_sector = (current_disk_block * DISK_BLOCK_SIZE) % FLASH_SECTOR_SIZE;
      memcpy(output_buffer + i * DISK_BLOCK_SIZE, write_cache_sector_info[flash_sector].cache_pointer + block_offset_within_sector, DISK_BLOCK_SIZE);
    }
    else
    {
      uint32_t offset = disk_block_to_flash_address(current_disk_block);
      memcpy(output_buffer + i * DISK_BLOCK_SIZE, (const void *)(XIP_BASE + offset), DISK_BLOCK_SIZE);
    }
  }

  return disk_blocks_to_read; // TODO check for OOB read
}

/**
 * \brief Write 1 flash sector to the write cache
 * \param input_buffer Buffer containing data to write
 * \param disk_block Starting disk block number to write to
 */
void flash_write(const uint8_t *input_buffer, const uint32_t disk_block)
{
  uint32_t flash_sector = (disk_block * DISK_BLOCK_SIZE) / FLASH_SECTOR_SIZE;

  if (write_cache_sector_info[flash_sector].cache_pointer == NULL)
  {
    write_cache_sector_info[flash_sector].cache_pointer = malloc(FLASH_SECTOR_SIZE);

    // If malloc fails, sync the flash buffer and try again
    if (write_cache_sector_info[flash_sector].cache_pointer == NULL)
    {
      flash_sync();
      write_cache_sector_info[flash_sector].cache_pointer = malloc(FLASH_SECTOR_SIZE);

      if (write_cache_sector_info[flash_sector].cache_pointer == NULL)
      {
        error(ERROR_CACHE_OOM, true);
        return; // Failed to allocate memory even after flushing cache
      }
    }

    currently_allocated_sectors++;

    // We must load the entire 4K sector into cache before modifying our 512B block
    uint32_t flash_address = disk_block_to_flash_address(flash_sector * (FLASH_SECTOR_SIZE / DISK_BLOCK_SIZE));
    uint32_t flash_sector_base = flash_address & ~(FLASH_SECTOR_SIZE - 1u);
    memcpy(write_cache_sector_info[flash_sector].cache_pointer, (const void *)(XIP_BASE + flash_sector_base), FLASH_SECTOR_SIZE);
  }

  uint32_t block_offset_within_sector = (disk_block * DISK_BLOCK_SIZE) % FLASH_SECTOR_SIZE;
  memcpy(write_cache_sector_info[flash_sector].cache_pointer + block_offset_within_sector, input_buffer, DISK_BLOCK_SIZE);
  write_cache_sector_info[flash_sector].dirty = true;
}
