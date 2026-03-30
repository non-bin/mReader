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

#include "config.h"
#include "gui.h"
#include "epaper.h"
#include <stdlib.h>
#include <string.h>

image_buffer_info_t image_buffer_info;

/**
 * \brief Initialise an image buffer and associated info, ready for draw calls
 * \param hardware_width Width in pixels, of the display in it's default orientation
 * \param hardware_height Height in pixels, of the display in it's default orientation
 * \param rotation Rotation angle
 * \param bits_per_pixel Pixel bits_per_pixel
 * \param mirror Mirroring setting
 * \return Pointer to the image buffer, to be passed to epaper functions
 */
uint8_t *gui_init(const uint16_t hardware_width, const uint16_t hardware_height, const gui_rotation_t rotation, const gui_bits_per_pixel_t bits_per_pixel, const gui_mirror_t mirror)
{
    uint8_t *const image_buffer = (uint8_t *)malloc(EPAPER_IMAGE_SIZE);

    image_buffer_info.image_buffer = NULL;
    image_buffer_info.image_buffer = image_buffer;

    image_buffer_info.hardware_width = hardware_width;
    image_buffer_info.hardware_height = hardware_height;
    image_buffer_info.hardware_width_in_bytes = (hardware_width % 8 == 0) ? (hardware_width / 8) : (hardware_width / 8 + 1);

    gui_set_mirror(mirror);
    gui_set_rotation(rotation);
    gui_set_bits_per_pixel(bits_per_pixel);

    return image_buffer;
}

/**
 * \brief Change the rotation angle
 */
void gui_set_rotation(const gui_rotation_t rotation)
{
    switch (rotation)
    {
    case GUI_ROTATE_0:
    case GUI_ROTATE_180:
        image_buffer_info.rotation = rotation;
        image_buffer_info.rotated_width = image_buffer_info.hardware_width;
        image_buffer_info.rotated_height = image_buffer_info.hardware_height;
        return;
    case GUI_ROTATE_90:
    case GUI_ROTATE_270:
        image_buffer_info.rotation = rotation;
        image_buffer_info.rotated_width = image_buffer_info.hardware_height;
        image_buffer_info.rotated_height = image_buffer_info.hardware_width;
        return;
    }

    // TODO debug
}

/**
 * \brief Change the mirroring state
 */
void gui_set_mirror(const gui_mirror_t mirror)
{
    switch (mirror)
    {
    case GUI_MIRROR_NONE:
    case GUI_MIRROR_HORIZONTAL:
    case GUI_MIRROR_VERTICAL:
    case GUI_MIRROR_BOTH:
        image_buffer_info.mirror = mirror;
        return;
    }

    // TODO debug
}

/**
 * \brief Change the pixel bits_per_pixel
 */
void gui_set_bits_per_pixel(const gui_bits_per_pixel_t bits_per_pixel)
{
    uint8_t modulo = 0;

    switch (bits_per_pixel)
    {
    case GUI_BITS_PER_PIXEL_2:
        modulo = 8;
        break;
    case GUI_BITS_PER_PIXEL_4:
        modulo = 4;
        break;
    case GUI_BITS_PER_PIXEL_7:
        modulo = 2;
        break;
    }

    if (modulo == 0)
        return; // TODO debug

    image_buffer_info.bits_per_pixel = bits_per_pixel;
    image_buffer_info.hardware_width_in_bytes = (image_buffer_info.hardware_width % modulo == 0) ? (image_buffer_info.hardware_width / modulo) : (image_buffer_info.hardware_width / modulo + 1);
}

/**
 * \brief Set the pixel at (x_point, y_point) to the specified color, according to rotation, mirroring, and bits_per_pixel.
 */
static void gui_draw_pixel(uint16_t x_point, uint16_t y_point, uint16_t color)
{
    if (x_point > image_buffer_info.rotated_width || y_point > image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    uint16_t x = 0, y = 0;
    switch (image_buffer_info.rotation)
    {
    case GUI_ROTATE_0:
        x = x_point;
        y = y_point;
        break;
    case GUI_ROTATE_90:
        x = image_buffer_info.hardware_width - y_point - 1;
        y = x_point;
        break;
    case GUI_ROTATE_180:
        x = image_buffer_info.hardware_width - x_point - 1;
        y = image_buffer_info.hardware_height - y_point - 1;
        break;
    case GUI_ROTATE_270:
        x = y_point;
        y = image_buffer_info.hardware_height - x_point - 1;
        break;
    }

    switch (image_buffer_info.mirror)
    {
    case GUI_MIRROR_NONE:
        break;
    case GUI_MIRROR_HORIZONTAL:
        x = image_buffer_info.hardware_width - x - 1;
        break;
    case GUI_MIRROR_VERTICAL:
        y = image_buffer_info.hardware_height - y - 1;
        break;
    case GUI_MIRROR_BOTH:
        x = image_buffer_info.hardware_width - x - 1;
        y = image_buffer_info.hardware_height - y - 1;
        break;
    }

    if (x > image_buffer_info.hardware_width || y > image_buffer_info.hardware_height)
    {
        // TODO debug
        return;
    }

    switch (image_buffer_info.bits_per_pixel)
    {
    case GUI_BITS_PER_PIXEL_2:
    {
        uint32_t address = x / 8 + y * image_buffer_info.hardware_width_in_bytes;
        uint8_t immage_buffer_byte = image_buffer_info.image_buffer[address];
        if (color == EPAPER_BLACK)
            image_buffer_info.image_buffer[address] = immage_buffer_byte & ~(0x80 >> (x % 8));
        else
            image_buffer_info.image_buffer[address] = immage_buffer_byte | (0x80 >> (x % 8));
        break;
    }
    case GUI_BITS_PER_PIXEL_4:
    {
        uint32_t address = x / 4 + y * image_buffer_info.hardware_width_in_bytes;
        color = color % 4; // Guaranteed color bits_per_pixel is 4  --- 0~3
        uint8_t immage_buffer_byte = image_buffer_info.image_buffer[address];

        immage_buffer_byte = immage_buffer_byte & (~(0xC0 >> ((x % 4) * 2))); // Clear first, then set value
        image_buffer_info.image_buffer[address] = immage_buffer_byte | ((color << 6) >> ((x % 4) * 2));
        break;
    }
    case GUI_BITS_PER_PIXEL_7:
    {
        uint32_t address = x / 2 + y * image_buffer_info.hardware_width_in_bytes;
        uint8_t immage_buffer_byte = image_buffer_info.image_buffer[address];
        immage_buffer_byte = immage_buffer_byte & (~(0xF0 >> ((x % 2) * 4))); // Clear first, then set value
        image_buffer_info.image_buffer[address] = immage_buffer_byte | ((color << 4) >> ((x % 2) * 4));
        break;
    }
    }
}

/**
 * \brief Fill the frame buffer with the specified color
 */
void gui_draw_fill(const uint16_t color)
{
    switch (image_buffer_info.bits_per_pixel)
    {
    case GUI_BITS_PER_PIXEL_2:
    case GUI_BITS_PER_PIXEL_4:
        for (uint16_t y = 0; y < image_buffer_info.hardware_height; y++)
        {
            for (uint16_t x = 0; x < image_buffer_info.hardware_width_in_bytes; x++)
            { // 8 pixel =  1 byte
                uint32_t address = x + y * image_buffer_info.hardware_width_in_bytes;
                image_buffer_info.image_buffer[address] = color;
            }
        }
        break;
    case GUI_BITS_PER_PIXEL_7:
        for (uint16_t y = 0; y < image_buffer_info.hardware_height; y++)
        {
            for (uint16_t x = 0; x < image_buffer_info.hardware_width_in_bytes; x++)
            {
                uint32_t address = x + y * image_buffer_info.hardware_width_in_bytes;
                image_buffer_info.image_buffer[address] = (color << 4) | color;
            }
        }
        break;
    }
}

/**
 * \brief copy the values from one image buffer (or a compiled bitmap) to another
 */
void gui_draw_bitmap(const unsigned char *image_buffer)
{
    uint32_t address = 0;

    for (uint16_t y = 0; y < image_buffer_info.hardware_height; y++)
    {
        for (uint16_t x = 0; x < image_buffer_info.hardware_width_in_bytes; x++)
        {
            address = x + y * image_buffer_info.hardware_width_in_bytes;
            image_buffer_info.image_buffer[address] = (unsigned char)image_buffer[address];
        }
    }
}

/**
 * \brief Draw a single dot to the frame buffer
 */
void gui_draw_point(const uint16_t x_point, const uint16_t y_point, const uint16_t color, const gui_dot_size_t dot_size, const gui_dot_style_t dot_style)
{
    if (x_point > image_buffer_info.rotated_width || y_point > image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    switch (dot_style)
    {
    case GUI_DOT_STYLE_AROUND:
        for (uint16_t x = 0; x < 2 * dot_size - 1; x++)
        {
            for (uint16_t y = 0; y < 2 * dot_size - 1; y++)
            {
                if (x_point + x - dot_size < 0 || y_point + y - dot_size < 0)
                    break;

                gui_draw_pixel(x_point + x - dot_size, y_point + y - dot_size, color);
            }
        }
        break;
    case GUI_DOT_STYLE_RIGHTUP:
        for (uint16_t x = 0; x < dot_size; x++)
        {
            for (uint16_t y = 0; y < dot_size; y++)
            {
                gui_draw_pixel(x_point + x - 1, y_point + y - 1, color);
            }
        }
        break;
    }
}

/**
 * \brief Draw a line to the frame buffer, from (x_start, y_start) to (x_end, y_end)
 * \param dotted Whether to draw a dotted (true) or solid (false) line
 */
void gui_draw_line(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color, const gui_dot_size_t line_width, const bool dotted)
{
    if (x_start > image_buffer_info.rotated_width || y_start > image_buffer_info.rotated_height || x_end > image_buffer_info.rotated_width || y_end > image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    uint16_t x = x_start;
    uint16_t y = y_start;
    int16_t dx = (x_end - x_start >= 0) ? (int16_t)(x_end - x_start) : (int16_t)(x_start - x_end);
    int16_t dy = (y_end - y_start <= 0) ? (int16_t)(y_end - y_start) : (int16_t)(y_start - y_end);

    int8_t x_increment_direction = (x_start < x_end) ? 1 : -1;
    int8_t y_increment_direction = (y_start < y_end) ? 1 : -1;

    int16_t cumulative_error = (int16_t)(dx + dy);
    uint8_t dotted_length = 0;

    for (;;)
    {
        dotted_length++;
        // Painted dotted line, 2 point is really virtual
        if (dotted && dotted_length % 3 == 0)
        {
            // Debug("LINE_DOTTED\r\n");
            gui_draw_point(x, y, GUI_IMAGE_BACKGROUND, line_width, GUI_DOT_STYLE_DEFAULT);
            dotted_length = 0;
        }
        else
        {
            gui_draw_point(x, y, color, line_width, GUI_DOT_STYLE_DEFAULT);
        }
        if (2 * cumulative_error >= dy)
        {
            if (x == x_end)
                break;
            cumulative_error = (int16_t)(cumulative_error + dy);
            x += x_increment_direction;
        }
        if (2 * cumulative_error <= dx)
        {
            if (y == y_end)
                break;
            cumulative_error = (int16_t)(cumulative_error + dx);
            y += y_increment_direction;
        }
    }
}

/**
 * \brief Draw a rectangle to the frame buffer, with corners at (x_start, y_start) and (x_end, y_end)
 * \param fill Whether to fill the rectangle (true) or just draw an outline (false)
 */
void gui_draw_rectangle(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end, const uint16_t color, const gui_dot_size_t line_width, const bool fill)
{
    if (x_start > image_buffer_info.rotated_width || y_start > image_buffer_info.rotated_height ||
        x_end > image_buffer_info.rotated_width || y_end > image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    if (fill)
    {
        for (uint16_t y = y_start; y < y_end; y++)
        {
            gui_draw_line(x_start, y, x_end, y, color, line_width, false);
        }
    }
    else
    {
        gui_draw_line(x_start, y_start, x_end, y_start, color, line_width, false);
        gui_draw_line(x_start, y_start, x_start, y_end, color, line_width, false);
        gui_draw_line(x_end, y_end, x_end, y_start, color, line_width, false);
        gui_draw_line(x_end, y_end, x_start, y_end, color, line_width, false);
    }
}

/**
 * \brief Use the 8-point method to draw a circle of the specified size, centered on (x_center, y_center)
 * \param fill Whether to fill the circle (true) or just draw an outline (false)
 */
void gui_draw_circle(const uint16_t x_center, const uint16_t y_center, const uint16_t radius, const uint16_t color, const gui_dot_size_t line_width, const bool fill)
{
    if (x_center > image_buffer_info.rotated_width || y_center >= image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    uint16_t x = 0;
    uint16_t y = (uint16_t)radius;

    int16_t cumulative_error = (int16_t)(3 - (radius << 1));

    if (fill)
    {
        while (x <= y)
        { // Realistic circles
            for (uint16_t sCountY = x; sCountY <= y; sCountY++)
            {
                gui_draw_point(x_center + x, y_center + sCountY, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 1
                gui_draw_point(x_center - x, y_center + sCountY, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 2
                gui_draw_point(x_center - sCountY, y_center + x, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 3
                gui_draw_point(x_center - sCountY, y_center - x, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 4
                gui_draw_point(x_center - x, y_center - sCountY, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 5
                gui_draw_point(x_center + x, y_center - sCountY, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 6
                gui_draw_point(x_center + sCountY, y_center - x, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT); // 7
                gui_draw_point(x_center + sCountY, y_center + x, color, GUI_DOT_SIZE_DEFAULT, GUI_DOT_STYLE_DEFAULT);
            }
            if (cumulative_error < 0)
                cumulative_error = (int16_t)(cumulative_error + (4 * x + 6));
            else
            {
                cumulative_error = (int16_t)(cumulative_error + (10 + 4 * (x - y)));
                y--;
            }
            x++;
        }
    }
    else
    { // Draw a hollow circle
        while (x <= y)
        {
            gui_draw_point(x_center + x, y_center + y, color, line_width, GUI_DOT_STYLE_DEFAULT); // 1
            gui_draw_point(x_center - x, y_center + y, color, line_width, GUI_DOT_STYLE_DEFAULT); // 2
            gui_draw_point(x_center - y, y_center + x, color, line_width, GUI_DOT_STYLE_DEFAULT); // 3
            gui_draw_point(x_center - y, y_center - x, color, line_width, GUI_DOT_STYLE_DEFAULT); // 4
            gui_draw_point(x_center - x, y_center - y, color, line_width, GUI_DOT_STYLE_DEFAULT); // 5
            gui_draw_point(x_center + x, y_center - y, color, line_width, GUI_DOT_STYLE_DEFAULT); // 6
            gui_draw_point(x_center + y, y_center - x, color, line_width, GUI_DOT_STYLE_DEFAULT); // 7
            gui_draw_point(x_center + y, y_center + x, color, line_width, GUI_DOT_STYLE_DEFAULT); // 0

            if (cumulative_error < 0)
                cumulative_error = (int16_t)(cumulative_error + 4 * x + 6);
            else
            {
                cumulative_error = (int16_t)(cumulative_error + 10 + 4 * (x - y));
                y--;
            }
            x++;
        }
    }
}

/**
 * \brief Draw a single ascii character to the frame buffer, with it's top left corner at (x_point, y_point)
 */
static void gui_draw_character(const uint16_t x_point, const uint16_t y_point, const char ascii_character, const font_t *font, const uint16_t foreground_color, const uint16_t background_color)
{
    const font_character_t character = font->characters[ascii_character - FONT_START_CHARACTER];
    if ((x_point + character.width) > image_buffer_info.rotated_width || (y_point + font->height) > image_buffer_info.rotated_height)
    {
        // TODO debug
        return;
    }

    const font_character_bitmap_row_t *bitmap_row_pointer = &character.bitmap[0];
    const font_character_bitmap_row_t first_column_mask = 1 << (character.width - 1);

    for (uint16_t row = 0; row < font->height; row++)
    {
        for (uint16_t column = 0; column < character.width; column++)
        {
            if (*bitmap_row_pointer & (first_column_mask >> column))
            {
                gui_draw_pixel(x_point + column, y_point + row, foreground_color);
            }
            else
            {
                gui_draw_pixel(x_point + column, y_point + row, background_color);
            }
        }
        bitmap_row_pointer++;
    }
}

/**
 * \brief Draw an ascii string to the frame buffer, with it's top left corner at (x_point, y_point)
 * \param x_start The x value that should be used if a word must be wrapped
 *
 * \warning x_point and y_point are updated to point to where the start of the next word should be
 */
static void gui_draw_word(uint16_t *x_point, uint16_t *y_point, const uint16_t x_start, const char *word, const font_t *font, const uint16_t foreground_color, const uint16_t background_color)
{
    while (*word != '\0')
    {
        char character = (*word >= FONT_START_CHARACTER && *word <= FONT_END_CHARACTER) ? *word : FONT_UNKNOWN_CHARACTER;

        // if X direction filled , reposition to(x_start,y_point),y_point is Y direction plus the height of the character
        if ((*x_point + font->characters[character - FONT_START_CHARACTER].width) > image_buffer_info.rotated_width)
        {
            *x_point = x_start;
            *y_point += font->height + font->line_spacing;
        }

        // If the Y direction is full
        if ((*y_point + font->height) > image_buffer_info.rotated_height)
        {
            return;
        }
        gui_draw_character(*x_point, *y_point, character, font, foreground_color, background_color);

        *x_point += font->characters[character - FONT_START_CHARACTER].width + font->character_spacing;
        word++;
    }
}

/**
 * \brief Draw an ascii string to the frame buffer, with it's top left corner at (x_start, y_start), wrapping at WORD_SEPARATORS
 * \return The y position that the next line may be drawn at
 */
uint16_t gui_draw_string(const uint16_t x_start, const uint16_t y_start, const char *string, const font_t *font, const uint16_t foreground_color, const uint16_t background_color)
{
    uint16_t x_point = x_start;
    uint16_t y_point = y_start;

    if (x_start > image_buffer_info.rotated_width || y_start > image_buffer_info.rotated_height)
    {
        // TODO debug
        return image_buffer_info.rotated_height;
    }

    char word[UINT16_MAX] = {0};
    uint16_t word_length = 0;
    uint16_t word_width = -font->character_spacing; // The width of the word, initialized to -character_spacing to offset the first character's character_spacing

    while (*string != '\0')
    {
        if (*string == '\n') // Newline
        {
            x_point = x_start;
            y_point += font->height + font->line_spacing;
        }
        else if (*string == '\r') // Carriage return (works as a zero width space)
        {
            // Ignore
        }
        else if (*string == '\t') // Tab
        {
            x_point += (font->characters[0].width + font->character_spacing) * TAB_SIZE;
        }
        else
        {
            const char character = (*string >= FONT_START_CHARACTER && *string <= FONT_END_CHARACTER) ? *string : FONT_UNKNOWN_CHARACTER;
            word[word_length++] = character;
            word_width += font->characters[character - FONT_START_CHARACTER].width + font->character_spacing;
        }
        string++;

        // If char is in WORD_SEPARATORS, clear the word buffer and print the word
        if (strchr(WORD_SEPARATORS, *string) || word_length >= UINT16_MAX - 1)
        {
            if (word_length > 0)
            {
                char *word_start = word;

                // Wrap if needed, but not if this is the first word
                if (x_point + word_width > image_buffer_info.rotated_width && !(y_point == y_start && x_point == x_start))
                {
                    x_point = x_start;
                    y_point += font->height + font->line_spacing;

                    while ((word_start[0] == FONT_START_CHARACTER || word_start[0] == '\t') && word_length > 0)
                    {
                        word_start++;
                    }
                }

                word[word_length] = '\0';
                gui_draw_word(&x_point, &y_point, x_start, word_start, font, foreground_color, background_color);
                word[0] = '\0';
                word_length = 0;
                word_width = -1 * (font->character_spacing);
            }
        }
    }

    return y_point + font->height + font->line_spacing;
}

#define NUMBER_ARRAY_LENGTH 16 // The largest uint32 is 4294967295 which is 11 chars
/**
 * \brief Draw a base 10 integer to the frame buffer, with it's top left corner at (x_start, y_start)
 */
uint16_t gui_draw_number(const uint16_t x_point, const uint16_t y_point, const int32_t number, const font_t *font, const uint16_t foreground_color, const uint16_t background_color)
{
    int16_t number_array_index = 0, forwards_array_index = 0;
    char forwards_array[NUMBER_ARRAY_LENGTH] = {0}, backwards_array[NUMBER_ARRAY_LENGTH] = {0};

    if (x_point > image_buffer_info.rotated_width || y_point > image_buffer_info.rotated_height)
    {
        // Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return image_buffer_info.rotated_width;
    }

    // Converts a number to a string
    int32_t mutable_number = number;
    while (mutable_number)
    {
        backwards_array[number_array_index] = mutable_number % 10 + '0';
        number_array_index++;
        mutable_number /= 10;
    }

    // The string is inverted
    while (number_array_index > 0)
    {
        forwards_array[forwards_array_index] = backwards_array[number_array_index - 1];
        forwards_array_index++;
        number_array_index--;
    }

    uint16_t x = x_point;
    uint16_t y = y_point;
    gui_draw_word(&x, &y, x_point, forwards_array, font, foreground_color, background_color);

    return y;
}
