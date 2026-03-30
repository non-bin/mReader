/**
 * \copyright 2019 by the Waveshare team <https://github.com/waveshare/Pico_ePaper_Code>
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
 * \copyright 2022 by hammadrauf <https://github.com/hammadrauf/pico_epd_1in54>
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#ifndef __GUI_H
#define __GUI_H

#include "../fonts/fonts.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * \brief The number of bits needed to represent a pixel
 */
typedef enum
{
    GUI_BITS_PER_PIXEL_2,
    GUI_BITS_PER_PIXEL_4,
    GUI_BITS_PER_PIXEL_7,
    GUI_BITS_PER_PIXEL_DEFAULT = GUI_BITS_PER_PIXEL_2
} gui_bits_per_pixel_t;

typedef enum
{
    GUI_ROTATE_0 = 0,
    GUI_ROTATE_90 = 90,
    GUI_ROTATE_180 = 180,
    GUI_ROTATE_270 = 270,
    GUI_ROTATE_DEFAULT = GUI_ROTATE_0
} gui_rotation_t;

typedef enum
{
    GUI_MIRROR_NONE,
    GUI_MIRROR_HORIZONTAL,
    GUI_MIRROR_VERTICAL,
    GUI_MIRROR_BOTH,
    GUI_MIRROR_DEFAULT = GUI_MIRROR_NONE
} gui_mirror_t;

typedef enum
{
    GUI_DOT_SIZE_1 = 1,
    GUI_DOT_SIZE_2,
    GUI_DOT_SIZE_3,
    GUI_DOT_SIZE_4,
    GUI_DOT_SIZE_5,
    GUI_DOT_SIZE_6,
    GUI_DOT_SIZE_7,
    GUI_DOT_SIZE_8,
    GUI_DOT_SIZE_DEFAULT = GUI_DOT_SIZE_1
} gui_dot_size_t;

// TODO comment
typedef enum
{
    GUI_DOT_STYLE_AROUND,
    GUI_DOT_STYLE_RIGHTUP,
    GUI_DOT_STYLE_DEFAULT = GUI_DOT_STYLE_AROUND
} gui_dot_style_t;

/**
 * \brief Represents a gui
 */
typedef struct
{
    uint8_t *image_buffer;               // Pointer to the image buffer, which is drawn to by gui_draw_* calls, and is passed to the display
    uint16_t hardware_width;             // Width in pixels, of the display in it's default orientation
    uint16_t hardware_height;            // Height in pixels, of the display in it's default orientation
    uint16_t hardware_width_in_bytes;    // The number of bytes needed to represent a row of pixels
    gui_rotation_t rotation;             // Rotation angle
    uint16_t rotated_width;              // Width in pixels, of the display after it has been rotated
    uint16_t rotated_height;             // Height in pixels, of the display after it has been rotated
    gui_mirror_t mirror;                 // Any mirroring to apply to draw calls
    gui_bits_per_pixel_t bits_per_pixel; // The number of bits needed to represent a pixel
} image_buffer_info_t;
extern image_buffer_info_t image_buffer_info;

#define GUI_IMAGE_BACKGROUND EPAPER_WHITE

uint8_t *gui_init(const uint16_t hardware_width, const uint16_t hardware_height, const gui_rotation_t rotation, const gui_bits_per_pixel_t bits_per_pixel, const gui_mirror_t mirror);

void gui_set_rotation(const gui_rotation_t rotation);
void gui_set_mirror(const gui_mirror_t mirror);
void gui_set_bits_per_pixel(const gui_bits_per_pixel_t bits_per_pixel);

void gui_draw_fill(const uint16_t color);
void gui_draw_bitmap(const unsigned char *image_buffer);
void gui_draw_point(const uint16_t x_point, const uint16_t y_point, const uint16_t color, const gui_dot_size_t dot_size, const gui_dot_style_t dot_style);
void gui_draw_line(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color, const gui_dot_size_t line_width, const bool dotted);
void gui_draw_rectangle(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color, const gui_dot_size_t line_width, const bool fill);
void gui_draw_circle(const uint16_t x_center, const uint16_t y_center, const uint16_t radius, const uint16_t color, const gui_dot_size_t line_width, const bool fill);
uint16_t gui_draw_string(const uint16_t x_start, const uint16_t y_start, const char *string, const font_t *font, const uint16_t foreground_color, const uint16_t background_color);
uint16_t gui_draw_number(const uint16_t x_point, const uint16_t y_point, const int32_t number, const font_t *font, const uint16_t foreground_color, const uint16_t background_color);

#endif /* __GUI_H */
