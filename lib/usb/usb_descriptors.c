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

#include "tusb.h"
#include "config.h"

#define USB_BCD 0x0200         // USB spec version (USB 2.0)
#define USB_LANGUAGE_ID 0x0409 // English US

// Indexes into the string descriptor table
#define STRING_INDEX_MANUFACTURER 0
#define STRING_INDEX_PRODUCT 1
#define STRING_INDEX_SERIAL 2
#define STRING_INDEX_MSC_INTERFACE 3

// Will be converted to UTF-16LE by tud_descriptor_string_cb
char const *string_descriptors[] =
    {
        "Alice Jacka", // 0: Manufacturer
        "mReader",     // 1: Product
        "000001",      // 2: Serial TODO generate from flash ID
        "USB Drive",   // 3: MSC interface string (optional)
};

tusb_desc_device_t const device_descriptor =
    {
        .bLength = sizeof(tusb_desc_device_t), // Size of this descriptor
        .bDescriptorType = TUSB_DESC_DEVICE,   // This is a device descriptor

        .bcdUSB = USB_BCD, // USB spec version

        .bDeviceClass = TUSB_CLASS_MISC, // We are a "Miscellaneous Device"
        .bDeviceSubClass = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol = MISC_PROTOCOL_IAD,

        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE, // Set "Root hub port 0" packet size

        .idVendor = USB_VID,      // Vendor ID
        .idProduct = USB_PID,     // Product ID
        .bcdDevice = VERSION_BCD, // mReader firmware version

        .iManufacturer = STRING_INDEX_MANUFACTURER, // Indexes into the string descriptor table
        .iProduct = STRING_INDEX_PRODUCT,
        .iSerialNumber = STRING_INDEX_SERIAL,

        .bNumConfigurations = 0x01, // Number of configurations the device supports
};

// USB interface numbers
enum
{
  ITF_NUM_MSC = 0,
  ITF_NUM_TOTAL // Length of this list
};

#define EPNUM_MSC_OUT 0x01 // 0b0 000 0001
#define EPNUM_MSC_IN 0x81  // 0b1 000 0001
//                    Direction ^
//                         Reserved ^
//                      Interface number ^

/**
 * \return Pointer to the device descriptor
 */
uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&device_descriptor;
}

uint8_t const configuration_descriptor[] =
    {
        TUD_CONFIG_DESCRIPTOR(1,                                        // 1 indexed configuration number (always 1 I think)
                              ITF_NUM_TOTAL,                            // Total number of configurations
                              0,                                        // string index (unused)
                              (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN), // Total length of `configuration_descriptor`
                              0,                                        // Don't need TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP
                              100                                       // mA current limit
                              ),

        TUD_MSC_DESCRIPTOR(ITF_NUM_MSC,                // Interface number
                           STRING_INDEX_MSC_INTERFACE, // String index for this interface
                           EPNUM_MSC_OUT,              // Output endpoint address
                           EPNUM_MSC_IN,               // Input endpoint address
                           CFG_TUD_MSC_EP_BUFSIZE      // Endpoint max packet size
                           ),
};

/**
 * \return Pointer to the configuration descriptor, which may be a different pointer for each configuration
 * \param index Which configuration the host asked for; we only support 1 so ignore this
 */
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  (void)index;
  return configuration_descriptor;
}

static uint16_t descriptor_string[32];
/**
 * \param index Which string the host asked for
 * \param language_id Requested language; we ignore it and always serve English.
 * \return Pointer to the requested string descriptor, encoded in UTF-16LE
 */
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t language_id)
{
  (void)language_id;

  uint8_t character_count = 0;

  if (index == 0)
  {
    // Language ID
    descriptor_string[1] = USB_LANGUAGE_ID;
    character_count = 1;
  }
  else
  {
    // Out of bounds
    if (index >= (sizeof(string_descriptors) / sizeof(string_descriptors[0])))
      return NULL;

    const char *string = string_descriptors[index];

    // Convert ASCII to UTF-16LE
    while (string[character_count] && (character_count < 31))
    {
      descriptor_string[1 + character_count] = (uint16_t)string[character_count];
      character_count++;
    }
  }

  // first word is length (in bytes) and descriptor type
  descriptor_string[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * character_count + 2));

  return descriptor_string;
}
