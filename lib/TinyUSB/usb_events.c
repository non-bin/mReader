#include "tusb.h"
#include <stdbool.h>

volatile bool usb_mounted = false;
volatile bool usb_just_unmounted = false;

void tud_mount_cb(void)
{
  usb_mounted = true;
}

void tud_umount_cb(void)
{
  usb_mounted = false;
  usb_just_unmounted = true;
}

void tud_suspend_cb(bool remote_wakeup_en)
{
  (void)remote_wakeup_en;
  usb_mounted = false;
  usb_just_unmounted = true;
}
