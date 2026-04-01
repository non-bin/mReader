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

#ifndef __FONTS_H_
#define __FONTS_H_

#define FONT_START_CHARACTER ' '
#define FONT_END_CHARACTER '~'
#define FONT_UNKNOWN_CHARACTER 0x7F
#define FONT_CHARACTERS_LENGTH FONT_END_CHARACTER - FONT_START_CHARACTER + 2

#include <stdint.h>

/**
 * \brief Calculate character width
 * \return A font_character_t
 */
#define CHAR(row1, ...)                                                      \
  {                                                                          \
    sizeof(#row1) - 3, (font_character_bitmap_row_t[]) { row1, __VA_ARGS__ } \
  }

typedef uint32_t font_character_bitmap_row_t;

/**
 * \brief Prepresents a character in a font
 */
typedef const struct
{
  const uint16_t width;                // Width of the character in pixels
  font_character_bitmap_row_t *bitmap; // Character bitmap, each value representing a row of pixels
} font_character_t;

typedef const font_character_t font_character_array_t[FONT_CHARACTERS_LENGTH];

/**
 * \brief Represents a font
 */
typedef const struct
{
  const font_character_t *characters; // Array of characters
  const uint16_t height;              // Height of all characters, in pixels
  const uint16_t character_spacing;   // Spacing to add between characters on the same line, in pixels
  const uint16_t line_spacing;        // Spacing to add between lines, in pixels
} font_t;

extern font_t font3;
extern font_t font4;
extern font_t font5;
extern font_t font6;
extern font_t font8;
extern font_t font12;
extern font_t font16;
extern font_t font24;
extern font_t font20;
#define FONT_COUNT 9
extern font_t *const fonts[FONT_COUNT];

#endif /* __FONTS_H_ */
