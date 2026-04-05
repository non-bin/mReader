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

#ifndef __TUSB_CONFIG_H_
#define __TUSB_CONFIG_H_

#define CFG_TUSB_MCU OPT_MCU_RP2040 // MCU Family

#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE // Set "Root hub port 0" to be a device
#define CFG_TUD_ENDPOINT0_SIZE 64             // Set "Root hub port 0" packet size. (64 is the default for full speed)

#define CFG_TUD_MSC 1             // Enable Mass Storage Class
#define CFG_TUD_MSC_EP_BUFSIZE 64 // Set the endpoint buffer size used by the MSC class for its bulk endpoints (64 is the default for full speed)

#endif /* __TUSB_CONFIG_H_ */
