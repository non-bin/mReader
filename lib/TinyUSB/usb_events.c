#include "tusb.h"
#include <stdbool.h>

volatile bool g_usb_mounted = false;
volatile bool g_usb_just_unmounted = false;

void tud_mount_cb(void)
{
  g_usb_mounted = true;
}

void tud_umount_cb(void)
{
  g_usb_mounted = false;
  g_usb_just_unmounted = true;
}

void tud_suspend_cb(bool remote_wakeup_en)
{
  (void)remote_wakeup_en;
  g_usb_mounted = false;
  g_usb_just_unmounted = true;
}
