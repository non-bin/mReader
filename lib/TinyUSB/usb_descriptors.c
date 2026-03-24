#include "tusb.h"
#include "pico/unique_id.h"
#include <string.h>

#define USB_VID 0xCAFE
#define USB_PID 0x4011
#define USB_BCD 0x0100

enum
{
  ITF_NUM_MSC = 0,
  ITF_NUM_TOTAL
};

#define EPNUM_MSC_OUT 0x01
#define EPNUM_MSC_IN (0x80 | 0x01)

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN)

tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = USB_BCD,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&desc_device;
}

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 4, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64),
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  (void)index;
  return desc_configuration;
}

static char serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];

static char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: English (0x0409)
    "mReader",                  // 1
    "mReader MSC",              // 2
    serial_str,                 // 3
    "mReader Disk"              // 4
};

static uint16_t _desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void)langid;

  if (!serial_str[0])
  {
    pico_get_unique_board_id_string(serial_str, sizeof(serial_str));
  }

  uint8_t chr_count;

  if (index == 0)
  {
    _desc_str[1] = 0x0409;
    chr_count = 1;
  }
  else
  {
    if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))
      return NULL;

    const char *str = string_desc_arr[index];
    chr_count = 0;
    while (str[chr_count] && chr_count < 31)
    {
      _desc_str[1 + chr_count] = (uint16_t)str[chr_count];
      chr_count++;
    }
  }

  _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
  return _desc_str;
}
