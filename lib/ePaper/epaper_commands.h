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
 *
 * https://files.waveshare.com/upload/e/e5/1.54inch_e-paper_V2_Datasheet.pdf page 20-24
 * https://files.waveshare.com/upload/7/77/1.54inch_e-Paper_Datasheet.pdf page 8 to 13
 */

#ifndef __EPAPER_COMMANDS_H_
#define __EPAPER_COMMANDS_H_

#define EPAPER_COMMAND_DRIVER_OUTPUT_CONTROL 0x01
#define EPAPER_COMMAND_SET_GATE_DRIVING_VOLTAGE 0x03
#define EPAPER_COMMAND_SET_SOURCE_DRIVING_VOLTAGE 0x04
#define EPAPER_COMMAND_DEEP_SLEEP 0x10
#define EPAPER_COMMAND_DATA_ENTRY_MODE 0x11
#define EPAPER_COMMAND_SW_RESET 0x12
#define EPAPER_COMMAND_MASTER_ACTIVATION 0x20
#define EPAPER_COMMAND_DISPLAY_UPDATE_CONTROL_1 0x21
#define EPAPER_COMMAND_DISPLAY_UPDATE_CONTROL_2 0x22
#define EPAPER_COMMAND_WRITE_RAM_BLACK_AND_WHITE 0x24
#define EPAPER_COMMAND_WRITE_RAM_RED 0x26
#define EPAPER_COMMAND_WRITE_VCOM_REGISTER 0x2C // change contrast?
#define EPAPER_COMMAND_WRITE_LOOK_UP_TABLE 0x32
#define EPAPER_COMMAND_BORDER_WAVEFROM 0x3C
#define EPAPER_COMMAND_SELECT_LOOK_UP_TABLE 0x3F
#define EPAPER_COMMAND_SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#define EPAPER_COMMAND_SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#define EPAPER_COMMAND_SET_RAM_X_ADDRESS_COUNTER 0x4E
#define EPAPER_COMMAND_SET_RAM_Y_ADDRESS_COUNTER 0x4F
// #define EPAPER_COMMAND_
// #define EPAPER_COMMAND_

#endif /* __EPAPER_COMMANDS_H_ */
