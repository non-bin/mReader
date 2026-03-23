/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#include "ws2812.pio.h"

PIO pio;
uint sm;
uint offset;

void ws2812_init(uint pin) {
  // This will find a free pio and state machine for our program and load it for us
  // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
  // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, pin, 1, true);
  hard_assert(success);

  ws2812_program_init(pio, sm, offset, pin, 800000, false);
}

void ws2812_deinit() {
  // This will free resources and unload our program
  pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}

static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return
    ((uint32_t) (r) << 8) |
    ((uint32_t) (g) << 16) |
    (uint32_t) (b);
}

void put_pixel(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t pixel_grb = urgb_u32(r, g, b);
  pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}
