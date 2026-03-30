/**
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \copyright 2020 by Raspberry Pi (Trading) Ltd <https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812>
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#include "ws2812.pio.h"
#include "config.h"

#define LED_BRIGHTNESS_MULTIPLIER LED_BRIGHTNESS_PERCENT / 100

PIO pio; // Programmable I/O
uint state_machine;
uint offset;

/**
 * \brief Find a free pio and state machine, and set them up to drive the led
 * \param pin The pin that the ws2812 data pin is connected to
 */
void ws2812_init(uint pin)
{
  // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
  // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &state_machine, &offset, pin, 1, true);
  hard_assert(success);

  ws2812_program_init(pio, state_machine, offset, pin, 800000, false);
}

/**
 * \brief Free resources and unload the ws2812 program
 */
void ws2812_deinit()
{
  pio_remove_program_and_unclaim_sm(&ws2812_program, pio, state_machine, offset);
}

/**
 * \brief Convert RGB values to a GRB byte
 */
static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)(r * LED_BRIGHTNESS_MULTIPLIER) << 8) |
         ((uint32_t)(g * LED_BRIGHTNESS_MULTIPLIER) << 16) |
         (uint32_t)(b * LED_BRIGHTNESS_MULTIPLIER);
}

/**
 * \brief Send a color to the led strip
 */
void put_pixel(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t pixel_grb = urgb_u32(r, g, b);
  pio_sm_put_blocking(pio, state_machine, pixel_grb << 8u);
}
