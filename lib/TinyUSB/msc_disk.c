#include "msc_disk.h"

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"

#include <string.h>

static inline uint32_t lba_to_flash_offset(uint32_t lba)
{
    return DISK_FLASH_OFFSET + lba * DISK_BLOCK_SIZE;
}

void msc_disk_read_block(uint32_t lba, void *dst512)
{
    uint32_t off = lba_to_flash_offset(lba);
    memcpy(dst512, (const void *)(XIP_BASE + off), DISK_BLOCK_SIZE);
}

// Write one 512-byte block using a 4KB read-modify-erase-program cycle
void msc_disk_write_block(uint32_t lba, const void *src512)
{
    uint32_t off = lba_to_flash_offset(lba);

    uint32_t sector_base = off & ~(FLASH_SECTOR_SIZE - 1u);
    uint32_t sector_off = off - sector_base;

    uint8_t sector_buf[FLASH_SECTOR_SIZE];
    memcpy(sector_buf, (const void *)(XIP_BASE + sector_base), FLASH_SECTOR_SIZE);
    memcpy(&sector_buf[sector_off], src512, DISK_BLOCK_SIZE);

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(sector_base, FLASH_SECTOR_SIZE);
    flash_range_program(sector_base, sector_buf, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);
}

bool msc_disk_is_formatted(void)
{
    const uint8_t *bs = (const uint8_t *)(XIP_BASE + DISK_FLASH_OFFSET);
    bool sig55aa = (bs[510] == 0x55 && bs[511] == 0xAA);
    bool has_jmp = (bs[0] == 0xEB || bs[0] == 0xE9);
    // also check bytes per sector = 512
    bool bps512 = (bs[11] == 0x00 && bs[12] == 0x02);
    return sig55aa && has_jmp && bps512;
}
