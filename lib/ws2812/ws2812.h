/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#ifndef WS2812_H
#define WS2812_H

#define LED_RED 128, 0, 0
#define LED_YELLOW 128, 128, 0
#define LED_GREEN 0, 128, 0
#define LED_CYAN 0, 128, 128
#define LED_BLUE 0, 0, 128
#define LED_PURPLE 64, 0, 128
#define LED_WHITE 64, 64, 64
#define LED_OFF 0, 0, 0

void ws2812_init(uint pin);
void ws2812_deinit();
void put_pixel(uint8_t r, uint8_t g, uint8_t b);

#endif
