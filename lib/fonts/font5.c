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
 * Inspired by <https://www.dafont.com/teeny-tiny-pixls.font> 2018 by Chequered Ink
 */

#include "fonts.h"

font_character_array_t font5_characters =
    {
// 0x20 Space
#ifdef ANGEL_KISSES
        CHAR(0b000,
             0b000,
             0b010,
             0b000,
             0b000),
#else
        CHAR(0b000,
             0b000,
             0b000,
             0b000,
             0b000),
#endif

        // 0x21 !
        CHAR(0b1,
             0b1,
             0b1,
             0b0,
             0b1),

        // 0x22 "
        CHAR(0b101,
             0b101,
             0b000,
             0b000,
             0b000),

// 0x23 #
#ifdef FONT5_WIDE_HASH
        CHAR(0b01010,
             0b11111,
             0b01010,
             0b11111,
             0b01010),
#else
        CHAR(0b101,
             0b111,
             0b101,
             0b111,
             0b101),
#endif

        // 0x24 $
        CHAR(0b01,
             0b11,
             0b10,
             0b01,
             0b11),

        // 0x25 %
        CHAR(0b101,
             0b001,
             0b010,
             0b100,
             0b101),

// 0x26 &
#ifdef FONT5_WIDE_AMPERSAND
        CHAR(0b0100,
             0b1000,
             0b0101,
             0b1010,
             0b0101),
#else
        CHAR(0b010,
             0b101,
             0b101,
             0b010,
             0b101),
#endif

        // 0x27 '
        CHAR(0b1,
             0b1,
             0b0,
             0b0,
             0b0),

        // 0x28 (
        CHAR(0b01,
             0b10,
             0b10,
             0b10,
             0b01),

        // 0x29 )
        CHAR(0b10,
             0b01,
             0b01,
             0b01,
             0b10),

        // 0x2A *
        CHAR(0b010,
             0b111,
             0b010,
             0b101,
             0b000),

        // 0x2B +
        CHAR(0b000,
             0b010,
             0b111,
             0b010,
             0b000),

        // 0x2C ,
        CHAR(0b00,
             0b00,
             0b00,
             0b01,
             0b10),

        // 0x2D -
        CHAR(0b000,
             0b000,
             0b111,
             0b000,
             0b000),

        // 0x2E .
        CHAR(0b0,
             0b0,
             0b0,
             0b0,
             0b1),

        // 0x2F /
        CHAR(0b001,
             0b010,
             0b010,
             0b100,
             0b100),

        // 0x30 0
        CHAR(0b111,
             0b101,
             0b101,
             0b101,
             0b111),

        // 0x31 1
        CHAR(0b010,
             0b110,
             0b010,
             0b010,
             0b111),

        // 0x32 2
        CHAR(0b111,
             0b001,
             0b111,
             0b100,
             0b111),

        // 0x33 3
        CHAR(0b111,
             0b001,
             0b011,
             0b001,
             0b111),

        // 0x34 4
        CHAR(0b101,
             0b101,
             0b111,
             0b001,
             0b001),

        // 0x35 5
        CHAR(0b111,
             0b100,
             0b111,
             0b001,
             0b111),

        // 0x36 6
        CHAR(0b111,
             0b100,
             0b111,
             0b101,
             0b111),

        // 0x37 7
        CHAR(0b111,
             0b101,
             0b001,
             0b001,
             0b001),

        // 0x38 8
        CHAR(0b111,
             0b101,
             0b111,
             0b101,
             0b111),

        // 0x39 9
        CHAR(0b111,
             0b101,
             0b111,
             0b001,
             0b001),

        // 0x3A :
        CHAR(0b0,
             0b1,
             0b0,
             0b1,
             0b0),

        // 0x3B ;
        CHAR(0b0,
             0b1,
             0b0,
             0b1,
             0b1),

        // 0x3C <
        CHAR(0b001,
             0b010,
             0b100,
             0b010,
             0b001),

        // 0x3D =
        CHAR(0b000,
             0b111,
             0b000,
             0b111,
             0b000),

        // 0x3E >
        CHAR(0b100,
             0b010,
             0b001,
             0b010,
             0b100),

        // 0x3F ?
        CHAR(0b111,
             0b001,
             0b011,
             0b000,
             0b010),

// 0x40 @
#ifdef FONT5_WIDE_AT
        CHAR(0b0110,
             0b1001,
             0b1010,
             0b1000,
             0b0110),
#else
        CHAR(0b111,
             0b101,
             0b101,
             0b100,
             0b111),
#endif

        // 0x41 A
        CHAR(0b111,
             0b101,
             0b111,
             0b101,
             0b101),

        // 0x42 B
        CHAR(0b111,
             0b101,
             0b110,
             0b101,
             0b111),

        // 0x43 C
        CHAR(0b111,
             0b100,
             0b100,
             0b100,
             0b111),

        // 0x44 D
        CHAR(0b110,
             0b101,
             0b101,
             0b101,
             0b110),

        // 0x45 E
        CHAR(0b111,
             0b100,
             0b110,
             0b100,
             0b111),

        // 0x46 F
        CHAR(0b111,
             0b100,
             0b110,
             0b100,
             0b100),

        // 0x47 G
        CHAR(0b111,
             0b100,
             0b101,
             0b101,
             0b111),

        // 0x48 H
        CHAR(0b101,
             0b101,
             0b111,
             0b101,
             0b101),

        // 0x49 I
        CHAR(0b111,
             0b010,
             0b010,
             0b010,
             0b111),

        // 0x4A J
        CHAR(0b001,
             0b001,
             0b001,
             0b101,
             0b111),

        // 0x4B K
        CHAR(0b101,
             0b101,
             0b110,
             0b101,
             0b101),

        // 0x4C L
        CHAR(0b100,
             0b100,
             0b100,
             0b100,
             0b111),

// 0x4D M
#ifdef FONT5_WIDE_UP_M
        CHAR(0b01010,
             0b10101,
             0b10101,
             0b10001,
             0b10001),
#else

        CHAR(0b101,
             0b111,
             0b101,
             0b101,
             0b101),
#endif

// 0x4E N
#ifdef FONT5_WIDE_UP_N
        CHAR(0b1001,
             0b1101,
             0b1011,
             0b1001,
             0b1001),
#else
        CHAR(0b110,
             0b101,
             0b101,
             0b101,
             0b101),
#endif

        // 0x4F O
        CHAR(0b111,
             0b101,
             0b101,
             0b101,
             0b111),

        // 0x50 P
        CHAR(0b111,
             0b101,
             0b111,
             0b100,
             0b100),

        // 0x51 Q
        CHAR(0b111,
             0b101,
             0b101,
             0b110,
             0b001),

        // 0x52 R
        CHAR(0b111,
             0b101,
             0b110,
             0b101,
             0b101),

        // 0x53 S
        CHAR(0b111,
             0b100,
             0b111,
             0b001,
             0b111),

        // 0x54 T
        CHAR(0b111,
             0b010,
             0b010,
             0b010,
             0b010),

        // 0x55 U
        CHAR(0b101,
             0b101,
             0b101,
             0b101,
             0b111),

        // 0x56 V
        CHAR(0b101,
             0b101,
             0b101,
             0b101,
             0b010),

        // 0x57 W
        CHAR(0b101,
             0b101,
             0b101,
             0b111,
             0b101),

        // 0x58 X
        CHAR(0b101,
             0b101,
             0b010,
             0b101,
             0b101),

        // 0x59 Y
        CHAR(0b101,
             0b101,
             0b010,
             0b010,
             0b010),

        // 0x5A Z
        CHAR(0b111,
             0b001,
             0b010,
             0b100,
             0b111),

        // 0x5B [
        CHAR(0b11,
             0b10,
             0b10,
             0b10,
             0b11),

        // 0x5C "\"
        CHAR(0b100,
             0b010,
             0b010,
             0b001,
             0b001),

        // 0x5D ]
        CHAR(0b11,
             0b01,
             0b01,
             0b01,
             0b11),

        // 0x5E ^
        CHAR(0b010,
             0b101,
             0b000,
             0b000,
             0b000),

        // 0x5F _
        CHAR(0b000,
             0b000,
             0b000,
             0b000,
             0b111),

        // 0x60 `
        CHAR(0b10,
             0b01,
             0b00,
             0b00,
             0b00),

        // 0x61 a
        CHAR(0b000,
             0b011,
             0b101,
             0b101,
             0b011),

        // 0x62 b
        CHAR(0b100,
             0b100,
             0b111,
             0b101,
             0b111),

        // 0x63 c
        CHAR(0b000,
             0b000,
             0b111,
             0b100,
             0b111),

        // 0x64 d
        CHAR(0b001,
             0b001,
             0b111,
             0b101,
             0b111),

        // 0x65 e
        CHAR(0b000,
             0b011,
             0b101,
             0b110,
             0b011),

        // 0x66 f
        CHAR(0b001,
             0b010,
             0b111,
             0b010,
             0b010),

        // 0x67 g
        CHAR(0b111,
             0b101,
             0b111,
             0b001,
             0b111),

        // 0x68 h
        CHAR(0b100,
             0b100,
             0b111,
             0b101,
             0b101),

        // 0x69 i
        CHAR(0b0,
             0b1,
             0b0,
             0b1,
             0b1),

        // 0x6A j
        CHAR(0b01,
             0b00,
             0b01,
             0b01,
             0b11),

        // 0x6B k
        CHAR(0b100,
             0b100,
             0b101,
             0b110,
             0b101),

        // 0x6C l
        CHAR(0b10,
             0b10,
             0b10,
             0b10,
             0b01),

        // 0x6D m
        CHAR(0b000,
             0b101,
             0b111,
             0b101,
             0b101),

        // 0x6E n
        CHAR(0b000,
             0b110,
             0b101,
             0b101,
             0b101),

        // 0x6F o
        CHAR(0b000,
             0b111,
             0b101,
             0b101,
             0b111),

        // 0x70 p
        CHAR(0b000,
             0b111,
             0b101,
             0b111,
             0b100),

        // 0x71 q
        CHAR(0b000,
             0b111,
             0b101,
             0b111,
             0b001),

        // 0x72 r
        CHAR(0b000,
             0b011,
             0b100,
             0b100,
             0b100),

        // 0x73 s
        CHAR(0b000,
             0b011,
             0b110,
             0b011,
             0b110),

        // 0x74 t
        CHAR(0b000,
             0b010,
             0b111,
             0b010,
             0b011),

        // 0x75 u
        CHAR(0b000,
             0b101,
             0b101,
             0b101,
             0b111),

        // 0x76 v
        CHAR(0b000,
             0b101,
             0b101,
             0b101,
             0b010),

        // 0x77 w
        CHAR(0b000,
             0b101,
             0b101,
             0b111,
             0b101),

        // 0x78 x
        CHAR(0b000,
             0b101,
             0b010,
             0b101,
             0b101),

        // 0x79 y
        CHAR(0b000,
             0b101,
             0b111,
             0b001,
             0b111),

        // 0x7A z
        CHAR(0b000,
             0b111,
             0b011,
             0b100,
             0b111),

        // 0x7B {
        CHAR(0b011,
             0b010,
             0b100,
             0b010,
             0b011),

        // 0x7C |
        CHAR(0b1,
             0b1,
             0b1,
             0b1,
             0b1),

        // 0x7D }
        CHAR(0b110,
             0b010,
             0b001,
             0b010,
             0b110),

        // 0x7E ~
        CHAR(0b000,
             0b001,
             0b111,
             0b100,
             0b000),

        // 0x7F UNKNOWN
        CHAR(0b1111,
             0b1111,
             0b1111,
             0b1111,
             0b1111),
};

font_t font5 =
    {
        font5_characters,
        5, // height
        1, // Character spacing
        1, // Line spacing
};
