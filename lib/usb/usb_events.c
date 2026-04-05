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

#include <stdbool.h>
#include "tusb.h"
#include "flash.h"

volatile bool usb_mounted = false;
volatile bool usb_just_unmounted = false;

/**
 * \brief Run when the drive is mounted
 */
void tud_mount_cb()
{
  usb_mounted = true;
}

/**
 * \brief Run when the drive is unmounted
 */
void tud_umount_cb()
{
  flash_sync();
  usb_mounted = false;
  usb_just_unmounted = true;
}

/**
 * \brief Run when the host suspends the USB connection
 */
void tud_suspend_cb(bool remote_wakeup_en)
{
  flash_sync();
  usb_mounted = false;
  usb_just_unmounted = true;
}
