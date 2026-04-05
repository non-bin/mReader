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

#ifndef __CONFIG_H_
#define __CONFIG_H_

///////////
// Debug //
///////////

// #define ANGEL_KISSES

#define DEBUG_LED           // Enable non-halting error codes
#define DEBUG_LED_ON_MS 300 // Leave these for halting errors
#define DEBUG_LED_OFF_MS 100

/////////////
// General //
/////////////

#define VERSION_STRING "v0.0.0" // eg "v2.14.3"
#define VERSION_BCD 0x0000      // eg 0x2143
#define HISTORY_LENGTH 4
#define SCROLL_SIZE 4
#define MAX_PATH_LENGTH 512

//////////
// Text //
//////////

#define DEFAULT_FONT font12
#define TAB_SIZE 2 // tabWidth = (space.width + characterGap) * TAB_SIZE
#define WORD_SEPARATORS " \r\n\t&*+-/<=>\\_~"
#define NEW_LINE_SPACING_MULTIPLIER 2

// #define FORMAT_DATA_INCLUDE_IB // Whether to include "iB" after data sizes, e.g. "1.2MiB" instead of "1.2M"

// Displayed to demo each font size
// #define FONT_PALLET " !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\b"
// #define FONT_PALLET "ABCDEFGHIJKLMNOPQRSTUVWXYZ\rabcdefghijklmnopqrstuvwxyz\r0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
// #define FONT_PALLET "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
// #define FONT_PALLET "AaBbCcDdEeFfGgHhIiJjKkLlMm\rNnOoPpQqRrSsTtUuVvWwXxYyZz\r0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
#define FONT_PALLET "Aa\rBb\rCc\rDd\rEe\rFf\rGg\rHh\rIi\rJj\rKk\rLl\rMm\rNn\rOo\rPp\rQq\rRr\rSs\rTt\rUu\rVv\rWw\rXx\rYy\rZz\n0123456789\r!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "

// #define FONT3_WIDE_M
// #define FONT3_WIDE_N
// #define FONT3_WIDE_Q
// #define FONT3_WIDE_W

// #define FONT4_WIDE_UP_M
#define FONT4_WIDE_UP_N
// #define FONT4_WIDE_UP_W
// #define FONT4_WIDE_LO_M
// #define FONT4_WIDE_LO_W

#define FONT5_WIDE_HASH
#define FONT5_WIDE_AMPERSAND
#define FONT5_WIDE_AT
// #define FONT5_WIDE_UP_M
#define FONT5_WIDE_UP_N

#define FONT6_WIDE_UP_M
#define FONT6_WIDE_UP_N
#define FONT6_WIDE_UP_Q
#define FONT6_WIDE_UP_W
#define FONT6_TALL_LO_G
// #define FONT6_TALL_LO_I
#define FONT6_WIDE_LO_W

/////////
// LED //
/////////

#define LED_BRIGHTNESS_PERCENT 12

#define LED_PIN 16

/////////////
// Buttons //
/////////////

#define BUTTON_DEFAULT_ACTIONS {BUTTON_NEXT, BUTTON_ENTER, BUTTON_BACK, BUTTON_PREVIOUS}

#define BUTTON_0_PIN 0
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 4
#define BUTTON_3_PIN 6
#define DEBOUNCE_TIME_MS 20
#define BUTTON_IRQ_EVENT GPIO_IRQ_EDGE_RISE // GPIO_IRQ_EDGE_FALL

/////////////
// Display //
/////////////

#define EPAPER_MODEL "1in54v2"
#define EPAPER_TIMEOUT_MS (10 * 1000)

#define EPAPER_SPI_PORT spi1
#define EPAPER_RST_PIN 12
#define EPAPER_DC_PIN 8
#define EPAPER_BUSY_PIN 13
#define EPAPER_CS_PIN 9
#define EPAPER_CLK_PIN 10
#define EPAPER_MOSI_PIN 11

/////////////
// Storage //
/////////////

// Onboard flash
// To see how much the binary uses, run `grep flash_binary build/mReader.elf.map`
// __flash_binary_start should be the same as XIP_BASE
#define FLASH_FIRMWARE_BYTES (256 * 1024)   // Last check, firmware used 129KiB, plus 127KiB headroom = 256KiB
#define FLASH_TOTAL_BYTES (2 * 1024 * 1024) // RP2040-Zero has 2 MiB flash

// Fat disk
#define DISK_BLOCK_SIZE 512
#define DISK_SIZE_BYTES (FLASH_TOTAL_BYTES - FLASH_FIRMWARE_BYTES)

/////////
// USB //
/////////

#define USB_VID 0xCafe // Vendor ID TODO change these
#define USB_PID 0x4005 // Product ID

#endif /* __CONFIG_H_ */
