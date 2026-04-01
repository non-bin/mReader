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

#include "error.h"
#include "config.h"
#include "ws2812.h"
#include "pico/stdlib.h" // sleep

// TODO make an async version
void flash_code(error_t error, uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1)
{
  if ((error) & (1 << 7))
    put_pixel(r1, g1, b1);
  else
    put_pixel(r0, g0, b0);
  sleep_ms(DEBUG_LED_ON_MS);

  for (char i = 1; i < 8; i++)
  {
    put_pixel(LED_OFF);
    sleep_ms(DEBUG_LED_OFF_MS);

    if ((error << i) & (1 << 7))
      put_pixel(r1, g1, b1);
    else
      put_pixel(r0, g0, b0);
    sleep_ms(DEBUG_LED_ON_MS);
  }

  put_pixel(LED_OFF);
}

void error(error_t error, bool halt)
{
#ifndef DEBUG_LED
  if (halt)
#endif
    flash_code(error, LED_RED, LED_PURPLE);

  while (halt)
  {
    sleep_ms(1000);
    flash_code(error, LED_RED, LED_PURPLE);
  }
}
