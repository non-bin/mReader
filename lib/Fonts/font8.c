/**
 ******************************************************************************
 * @file    Font8.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-February-2014
 * @brief   This file provides text Font8 for STM32xx-EVAL's LCD driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

//
//  Font data for Courier New 12pt
//

#include "fonts.h"

const font_character_t font8_characters[] =
		{
// 0x20 Space
#ifdef ANGEL_KISSES
				CHAR(
						0b00000,
						0b00000,
						0b00000,
						0b00100,
						0b00000,
						0b00000,
						0b00000,
						0b00000),
#else
				CHAR(
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000),
#endif

				// 0x21 !
				CHAR(
						0b1,
						0b1,
						0b1,
						0b1,
						0b0,
						0b1,
						0b0,
						0b0),

				// 0x22 "
				CHAR(
						0b101,
						0b101,
						0b000,
						0b000,
						0b000,
						0b000,
						0b000,
						0b000),

				// 0x23 #
				CHAR(
						0b00101,
						0b01010,
						0b11111,
						0b01010,
						0b11111,
						0b01010,
						0b10100,
						0b00000),

				// 0x24 $
				CHAR(
						0b010,
						0b011,
						0b110,
						0b011,
						0b001,
						0b110,
						0b010,
						0b000),

				// 0x25 %
				CHAR(
						0b0100,
						0b0100,
						0b0011,
						0b1100,
						0b0010,
						0b0010,
						0b0000,
						0b0000),

				// 0x26 &
				CHAR(
						0b01000,
						0b10100,
						0b01000,
						0b10101,
						0b10010,
						0b01101,
						0b00000,
						0b00000),

				// 0x27 '
				CHAR(
						0b1,
						0b1,
						0b1,
						0b0,
						0b0,
						0b0,
						0b0,
						0b0),

				// 0x28 (
				CHAR(
						0b01,
						0b10,
						0b10,
						0b10,
						0b10,
						0b10,
						0b01,
						0b00),

				// 0x29 )
				CHAR(
						0b10,
						0b01,
						0b01,
						0b01,
						0b01,
						0b01,
						0b10,
						0b00),

				// 0x2A *
				CHAR(
						0b010,
						0b111,
						0b010,
						0b101,
						0b000,
						0b000,
						0b000,
						0b000),

				// 0x2B +
				CHAR(
						0b00000,
						0b00100,
						0b00100,
						0b11111,
						0b00100,
						0b00100,
						0b00000,
						0b00000),

				// 0x2C ,
				CHAR(
						0b00,
						0b00,
						0b00,
						0b00,
						0b01,
						0b10,
						0b10,
						0b00),

				// 0x2D -
				CHAR(
						0b000,
						0b000,
						0b000,
						0b111,
						0b000,
						0b000,
						0b000,
						0b000),

				// 0x2E .
				CHAR(
						0b0,
						0b0,
						0b0,
						0b0,
						0b0,
						0b1,
						0b0,
						0b0),

				// 0x2F /
				CHAR(
						0b0001,
						0b0010,
						0b0010,
						0b0010,
						0b0100,
						0b0100,
						0b1000,
						0b0000),

				// 0x30 0
				CHAR(
						0b010,
						0b101,
						0b101,
						0b101,
						0b101,
						0b010,
						0b000,
						0b000),

				// 0x31 1
				CHAR(
						0b01100,
						0b00100,
						0b00100,
						0b00100,
						0b00100,
						0b11111,
						0b00000,
						0b00000),

				// 0x32 2
				CHAR(
						0b010,
						0b101,
						0b010,
						0b010,
						0b100,
						0b111,
						0b000,
						0b000),

				// 0x33 3
				CHAR(
						0b010,
						0b101,
						0b001,
						0b010,
						0b001,
						0b110,
						0b000,
						0b000),

				// 0x34 4
				CHAR(
						0b0010,
						0b0110,
						0b1010,
						0b1111,
						0b0010,
						0b0111,
						0b0000,
						0b0000),

				// 0x35 5
				CHAR(
						0b111,
						0b100,
						0b110,
						0b001,
						0b101,
						0b010,
						0b000,
						0b000),

				// 0x36 6
				CHAR(
						0b011,
						0b100,
						0b110,
						0b101,
						0b101,
						0b110,
						0b000,
						0b000),

				// 0x37 7
				CHAR(
						0b111,
						0b101,
						0b001,
						0b010,
						0b010,
						0b010,
						0b000,
						0b000),

				// 0x38 8
				CHAR(
						0b010,
						0b101,
						0b010,
						0b101,
						0b101,
						0b010,
						0b000,
						0b000),

				// 0x39 9
				CHAR(
						0b011,
						0b101,
						0b101,
						0b011,
						0b001,
						0b110,
						0b000,
						0b000),

				// 0x3A :
				CHAR(
						0b0,
						0b0,
						0b1,
						0b0,
						0b0,
						0b1,
						0b0,
						0b0),

				// 0x3B ;
				CHAR(
						0b00,
						0b00,
						0b01,
						0b00,
						0b01,
						0b10,
						0b00,
						0b00),

				// 0x3C <
				CHAR(
						0b0000,
						0b0001,
						0b0010,
						0b1100,
						0b0010,
						0b0001,
						0b0000,
						0b0000),

				// 0x3D =
				CHAR(
						0b000,
						0b111,
						0b000,
						0b111,
						0b000,
						0b000,
						0b000,
						0b000),

				// 0x3E >
				CHAR(
						0b0000,
						0b1000,
						0b0100,
						0b0011,
						0b0100,
						0b1000,
						0b0000,
						0b0000),

				// 0x3F ?
				CHAR(
						0b010,
						0b101,
						0b001,
						0b010,
						0b000,
						0b010,
						0b000,
						0b000),

				// 0x40 @
				CHAR(
						0b0110,
						0b1001,
						0b1001,
						0b1011,
						0b1001,
						0b1000,
						0b0111,
						0b0000),

				// 0x41 A
				CHAR(
						0b01100,
						0b00100,
						0b01010,
						0b01110,
						0b10001,
						0b11011,
						0b00000,
						0b00000),

				// 0x42 B
				CHAR(
						0b11110,
						0b01001,
						0b01110,
						0b01001,
						0b01001,
						0b11110,
						0b00000,
						0b00000),

				// 0x43 C
				CHAR(
						0b111,
						0b101,
						0b100,
						0b100,
						0b100,
						0b011,
						0b000,
						0b000),

				// 0x44 D
				CHAR(
						0b11110,
						0b01001,
						0b01001,
						0b01001,
						0b01001,
						0b11110,
						0b00000,
						0b00000),

				// 0x45 E
				CHAR(
						0b11111,
						0b01001,
						0b01100,
						0b01000,
						0b01001,
						0b11111,
						0b00000,
						0b00000),

				// 0x46 F
				CHAR(
						0b11111,
						0b01001,
						0b01100,
						0b01000,
						0b01000,
						0b11100,
						0b00000,
						0b00000),

				// 0x47 G
				CHAR(
						0b1110,
						0b1000,
						0b1000,
						0b1011,
						0b1010,
						0b0110,
						0b0000,
						0b0000),

				// 0x48 H
				CHAR(
						0b11101,
						0b01001,
						0b01111,
						0b01001,
						0b01001,
						0b11101,
						0b00000,
						0b00000),

				// 0x49 I
				CHAR(
						0b111,
						0b010,
						0b010,
						0b010,
						0b010,
						0b111,
						0b000,
						0b000),

				// 0x4A J
				CHAR(
						0b0111,
						0b0010,
						0b0010,
						0b1010,
						0b1010,
						0b0100,
						0b0000,
						0b0000),

				// 0x4B K
				CHAR(
						0b11011,
						0b01010,
						0b01100,
						0b01110,
						0b01010,
						0b11011,
						0b00000,
						0b00000),

				// 0x4C L
				CHAR(
						0b11100,
						0b01000,
						0b01000,
						0b01000,
						0b01001,
						0b11111,
						0b00000,
						0b00000),

				// 0x4D M
				CHAR(
						0b11011,
						0b11011,
						0b11011,
						0b10101,
						0b10001,
						0b11011,
						0b00000,
						0b00000),

				// 0x4E N
				CHAR(
						0b11011,
						0b01101,
						0b01101,
						0b01011,
						0b01011,
						0b11101,
						0b00000,
						0b00000),

				// 0x4F O
				CHAR(
						0b0110,
						0b1001,
						0b1001,
						0b1001,
						0b1001,
						0b0110,
						0b0000,
						0b0000),

				// 0x50 P
				CHAR(
						0b11110,
						0b01001,
						0b01001,
						0b01110,
						0b01000,
						0b11100,
						0b00000,
						0b00000),

				// 0x51 Q
				CHAR(
						0b0110,
						0b1001,
						0b1001,
						0b1001,
						0b1001,
						0b0110,
						0b0011,
						0b0000),

				// 0x52 R
				CHAR(
						0b11110,
						0b01001,
						0b01001,
						0b01110,
						0b01001,
						0b11101,
						0b00000,
						0b00000),

				// 0x53 S
				CHAR(
						0b111,
						0b100,
						0b010,
						0b001,
						0b101,
						0b111,
						0b000,
						0b000),

				// 0x54 T
				CHAR(
						0b11111,
						0b10101,
						0b00100,
						0b00100,
						0b00100,
						0b01110,
						0b00000,
						0b00000),

				// 0x55 U
				CHAR(
						0b11011,
						0b01001,
						0b01001,
						0b01001,
						0b01001,
						0b00110,
						0b00000,
						0b00000),

				// 0x56 V
				CHAR(
						0b11011,
						0b10001,
						0b01001,
						0b01010,
						0b01010,
						0b00110,
						0b00000,
						0b00000),

				// 0x57 W
				CHAR(
						0b11011,
						0b10001,
						0b10101,
						0b10101,
						0b10101,
						0b01010,
						0b00000,
						0b00000),

				// 0x58 X
				CHAR(
						0b11011,
						0b01010,
						0b00100,
						0b00100,
						0b01010,
						0b11011,
						0b00000,
						0b00000),

				// 0x59 Y
				CHAR(
						0b11011,
						0b10001,
						0b01010,
						0b00100,
						0b00100,
						0b01110,
						0b00000,
						0b00000),

				// 0x5A Z
				CHAR(
						0b1111,
						0b1001,
						0b0010,
						0b0100,
						0b1001,
						0b1111,
						0b0000,
						0b0000),

				// 0x5B [
				CHAR(
						0b11,
						0b10,
						0b10,
						0b10,
						0b10,
						0b10,
						0b11,
						0b00),

				// 0x5C "\"
				CHAR(
						0b1000,
						0b0100,
						0b0100,
						0b0010,
						0b0010,
						0b0010,
						0b0001,
						0b0000),

				// 0x5D ]
				CHAR(
						0b11,
						0b01,
						0b01,
						0b01,
						0b01,
						0b01,
						0b11,
						0b00),

				// 0x5E ^
				CHAR(
						0b010,
						0b010,
						0b101,
						0b000,
						0b000,
						0b000,
						0b000,
						0b000),

				// 0x5F _
				CHAR(
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b00000,
						0b11111),

				// 0x60 `
				CHAR(
						0b10,
						0b01,
						0b00,
						0b00,
						0b00,
						0b00,
						0b00,
						0b00),

				// 0x61 a
				CHAR(
						0b0000,
						0b0000,
						0b0110,
						0b0010,
						0b1110,
						0b1111,
						0b0000,
						0b0000),

				// 0x62 b
				CHAR(
						0b11000,
						0b01000,
						0b01110,
						0b01001,
						0b01001,
						0b11110,
						0b00000,
						0b00000),

				// 0x63 c
				CHAR(
						0b000,
						0b000,
						0b111,
						0b100,
						0b100,
						0b111,
						0b000,
						0b000),

				// 0x64 d
				CHAR(
						0b0011,
						0b0001,
						0b0111,
						0b1001,
						0b1001,
						0b0111,
						0b0000,
						0b0000),

				// 0x65 e
				CHAR(
						0b000,
						0b000,
						0b111,
						0b111,
						0b100,
						0b011,
						0b000,
						0b000),

				// 0x66 f
				CHAR(
						0b001,
						0b010,
						0b111,
						0b010,
						0b010,
						0b111,
						0b000,
						0b000),

				// 0x67 g
				CHAR(
						0b0000,
						0b0000,
						0b0111,
						0b1001,
						0b1001,
						0b0111,
						0b0001,
						0b0110),

				// 0x68 h
				CHAR(
						0b11000,
						0b01000,
						0b01110,
						0b01001,
						0b01001,
						0b11101,
						0b00000,
						0b00000),

				// 0x69 i
				CHAR(
						0b010,
						0b000,
						0b110,
						0b010,
						0b010,
						0b111,
						0b000,
						0b000),

				// 0x6A j
				CHAR(
						0b010,
						0b000,
						0b111,
						0b001,
						0b001,
						0b001,
						0b001,
						0b111),

				// 0x6B k
				CHAR(
						0b11000,
						0b01000,
						0b01011,
						0b01110,
						0b01010,
						0b11011,
						0b00000,
						0b00000),

				// 0x6C l
				CHAR(
						0b110,
						0b010,
						0b010,
						0b010,
						0b010,
						0b111,
						0b000,
						0b000),

				// 0x6D m
				CHAR(
						0b00000,
						0b00000,
						0b11010,
						0b10101,
						0b10101,
						0b10101,
						0b00000,
						0b00000),

				// 0x6E n
				CHAR(
						0b00000,
						0b00000,
						0b11110,
						0b01001,
						0b01001,
						0b11001,
						0b00000,
						0b00000),

				// 0x6F o
				CHAR(
						0b0000,
						0b0000,
						0b0110,
						0b1001,
						0b1001,
						0b0110,
						0b0000,
						0b0000),

				// 0x70 p
				CHAR(
						0b00000,
						0b00000,
						0b11110,
						0b01001,
						0b01001,
						0b01110,
						0b01000,
						0b11100),

				// 0x71 q
				CHAR(
						0b0000,
						0b0000,
						0b0111,
						0b1001,
						0b1001,
						0b0111,
						0b0001,
						0b0011),

				// 0x72 r
				CHAR(
						0b0000,
						0b0000,
						0b1111,
						0b0100,
						0b0100,
						0b1110,
						0b0000,
						0b0000),

				// 0x73 s
				CHAR(
						0b000,
						0b000,
						0b011,
						0b010,
						0b001,
						0b110,
						0b000,
						0b000),

				// 0x74 t
				CHAR(
						0b00000,
						0b01000,
						0b11110,
						0b01000,
						0b01001,
						0b00110,
						0b00000,
						0b00000),

				// 0x75 u
				CHAR(
						0b00000,
						0b00000,
						0b11011,
						0b01001,
						0b01001,
						0b00111,
						0b00000,
						0b00000),

				// 0x76 v
				CHAR(
						0b00000,
						0b00000,
						0b11001,
						0b01001,
						0b00110,
						0b00110,
						0b00000,
						0b00000),

				// 0x77 w
				CHAR(
						0b00000,
						0b00000,
						0b11011,
						0b10101,
						0b10101,
						0b01010,
						0b00000,
						0b00000),

				// 0x78 x
				CHAR(
						0b0000,
						0b0000,
						0b1001,
						0b0110,
						0b0110,
						0b1001,
						0b0000,
						0b0000),

				// 0x79 y
				CHAR(
						0b00000,
						0b00000,
						0b11011,
						0b01010,
						0b01010,
						0b00100,
						0b00100,
						0b01100),

				// 0x7A z
				CHAR(
						0b0000,
						0b0000,
						0b1111,
						0b1010,
						0b0101,
						0b1111,
						0b0000,
						0b0000),

				// 0x7B {
				CHAR(
						0b001,
						0b010,
						0b010,
						0b110,
						0b010,
						0b010,
						0b001,
						0b000),

				// 0x7C |
				CHAR(
						0b1,
						0b1,
						0b1,
						0b1,
						0b1,
						0b1,
						0b1,
						0b0),

				// 0x7D }
				CHAR(
						0b100,
						0b010,
						0b010,
						0b011,
						0b010,
						0b010,
						0b100,
						0b000),

				// 0x7E ~
				CHAR(
						0b0000,
						0b0000,
						0b0000,
						0b0101,
						0b1010,
						0b0000,
						0b0000,
						0b0000),

				// 0x7F UNKNOWN
				CHAR(
						0b11111,
						0b11111,
						0b11111,
						0b11111,
						0b11111,
						0b11111,
						0b11111,
						0b11111),

};

font_t font8 =
		{
				font8_characters,
				8, // Height
				1, // Character spacing
				2, // Line spacing
};
