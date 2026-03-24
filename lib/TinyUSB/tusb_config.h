#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// RP2040 + TinyUSB device config

#define CFG_TUSB_MCU OPT_MCU_RP2040
#define CFG_TUSB_OS OPT_OS_PICO
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE

#define CFG_TUD_ENABLED 1

// We only need MSC (Mass Storage)
#define CFG_TUD_MSC 1

// No CDC, HID, etc.
#define CFG_TUD_CDC 0
#define CFG_TUD_HID 0
#define CFG_TUD_MIDI 0
#define CFG_TUD_VENDOR 0

// Endpoint sizes
#define CFG_TUD_MSC_EP_BUFSIZE 512

#endif
