/**
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright 2019 by the Waveshare team <https://github.com/waveshare/Pico_ePaper_Code>
 * \copyright 2022 by hammadrauf <https://github.com/hammadrauf/pico_epd_1in54>
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#ifndef __EPAPER_H_
#define __EPAPER_H_

#include <stdint.h>
#include <stdbool.h>

#define EPAPER_WIDTH 200
#define EPAPER_HEIGHT 200
#define EPAPER_BLACK 0x00
#define EPAPER_WHITE 0xFF

#define EPAPER_WIDTH_BYTES ((EPAPER_WIDTH % 8 == 0) ? (EPAPER_WIDTH / 8) : (EPAPER_WIDTH / 8 + 1))
#define EPAPER_IMAGE_SIZE (EPAPER_WIDTH_BYTES * EPAPER_HEIGHT)

bool epaper_is_busy(void);
void epaper_init(void);
void epaper_partial_init(void);
void epaper_clear(uint16_t color);
void epaper_display(const uint8_t *image, const bool partial);
void epaper_partial_upload_base_image(const uint8_t *image);
void epaper_sleep(void);

#endif /* __EPAPER_H_ */
