/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#ifndef WS2812_H
#define WS2812_H

void ws2812_init(uint pin);
void ws2812_deinit();
void put_pixel(uint8_t r, uint8_t g, uint8_t b);

#endif
