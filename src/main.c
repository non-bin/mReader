#include "config.h"

#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"

#include "tusb.h"
#include "msc_disk.h"
#include "EPD_Test.h"
#include "EPD_1in54_V2.h"
#include "ws2812.h"

absolute_time_t last_screen_update = {0};
char *text;

absolute_time_t last_gpio_event_time[8] = {0};
bool button_pressed[8] = {false};

extern volatile bool g_usb_just_unmounted;
extern volatile bool g_usb_mounted;

void usb_storage_task()
{
  tud_task(); // TinyUSB device task

  if (g_usb_just_unmounted)
  {
    g_usb_just_unmounted = false;

    // Get a list of files in the root directory
    msc_file_info_t files[10];
    int count = msc_disk_get_file_list(files, 10);

    if (count > 0)
    {
      static uint8_t file_buf[1024];
      int read_len = msc_disk_read_file(files[0].start_cluster, files[0].size, file_buf, sizeof(file_buf) - 1);
      file_buf[read_len] = '\0';
      text = (char *)file_buf;
    }
  }
}

void gpio_callback(uint gpio, uint32_t events)
{
  if ((events & GPIO_IRQ_EDGE_RISE) && (events & GPIO_IRQ_EDGE_FALL))
  {
    return;
  }

  absolute_time_t now = get_absolute_time();

  if (absolute_time_diff_us(last_gpio_event_time[gpio], now) > DEBOUNCE_TIME_MS * 1000)
  {
    if (events & GPIO_IRQ_EDGE_RISE && !button_pressed[gpio])
    {
      button_pressed[gpio] = true;
      if (gpio == BUTTON_1_PIN)
        put_pixel(LED_RED);
      else if (gpio == BUTTON_2_PIN)
        put_pixel(LED_GREEN);
      else if (gpio == BUTTON_3_PIN)
        rom_reset_usb_boot_extra(-1, 0, 0); // Reset to BOOTSEL
      else if (gpio == BUTTON_4_PIN)
        put_pixel(LED_BLUE);
    }
    else if (events & GPIO_IRQ_EDGE_FALL && button_pressed[gpio])
    {
      button_pressed[gpio] = false;
    }
  }

  last_gpio_event_time[gpio] = now;
}

int main()
{
  text = malloc(1024);

  // Initialize WS2812 LED
  ws2812_init(WS2812_PIN);
  put_pixel(LED_OFF);

  msc_disk_lazy_format_fat16(); // Ensure filesystem exists
  tusb_init();                  // Initialize TinyUSB

  // Register button interrupts
  gpio_init(BUTTON_1_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_1_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_2_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_2_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_3_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_3_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_4_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_4_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

  epaper_init();
  // epaper_clear();

  uint8_t *imageCache = epaper_create_image_buffer();
  epaper_select_image_buffer(imageCache);

  // epaper_draw_bitmap(gImage_1IN54);
  // epaper_draw_bitmap(gImage_1IN54_2);

  uint16_t FG_COLOUR = BLACK;
  uint16_t BG_COLOUR = WHITE;
  absolute_time_t now = {0};

  while (1)
  {
    now = get_absolute_time();
    usb_storage_task();

    if (absolute_time_diff_us(last_screen_update, now) > 5 * 1000 * 1000 || last_screen_update == 0) // 5 seconds
    {
      last_screen_update = now;

      epaper_fill(BG_COLOUR);
      epaper_draw_string(5, 5, text, &FONT, FG_COLOUR, BG_COLOUR);

      epaper_display(imageCache);

      uint16_t temp = FG_COLOUR;
      FG_COLOUR = BG_COLOUR;
      BG_COLOUR = temp;
    }
  }

  // EPD_1in54_V2_test();

  // uint8_t *BlackImage;
  // uint16_t Imagesize = ((EPD_1IN54_V2_WIDTH % 8 == 0) ? (EPD_1IN54_V2_WIDTH / 8) : (EPD_1IN54_V2_WIDTH / 8 + 1)) * EPD_1IN54_V2_HEIGHT;
  // if ((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL)
  // {
  //   // printf("Failed to apply for black memory...\n");
  //   return -1;
  // }

  // // The image of the previous frame must be uploaded, otherwise the
  // // first few seconds will display an exception.
  // EPD_1IN54_V2_DisplayPartBaseImage(BlackImage);
  // Paint_SelectImage(BlackImage);
  // Paint_Clear(WHITE);
  // // enter partial mode
  // EPD_1IN54_V2_Init_Partial();
  // // printf("Partial refresh\n");

  // int xpos = 0;
  // int ypos = 0;
  // int xinc = 17;
  // int yinc = 24;
  // int xfact = 1;
  // int yfact = 1;
  // int ledval = 1;
  // int countdown = 100;
  // bool lastTime = true;
  // while (true)
  // {

  //   // Animate the character (countdown) times.
  //   if (countdown > 0)
  //   {
  //     // sleep_ms(250);
  //     Paint_ClearWindows(xpos, ypos, xpos + Font24.Width, ypos + Font24.Height, WHITE);
  //     Paint_DrawChar(xpos, ypos, 'A', &Font24, BLACK, WHITE);
  //     EPD_1IN54_V2_DisplayPart(BlackImage);
  //     sleep_ms(100);

  //     Paint_ClearWindows(xpos, ypos, xpos + Font24.Width, ypos + Font24.Height, WHITE);
  //     Paint_DrawChar(xpos, ypos, ' ', &Font24, BLACK, WHITE);
  //     // sleep_ms(2000);
  //     EPD_1IN54_V2_DisplayPart(BlackImage);
  //     sleep_ms(100);

  //     if (xpos >= 0 && xpos + (xfact * xinc) <= EPD_1IN54_V2_WIDTH - 1)
  //       xfact = xfact;
  //     if (xpos >= 0 && xpos + (xfact * xinc) > EPD_1IN54_V2_WIDTH - 1)
  //       xfact = -1 * xfact;
  //     if (xpos < 0)
  //       xfact = -1 * xfact;
  //     // Update x-position
  //     xpos = xpos + (xfact * xinc);

  //     if (ypos >= 0 && ypos + (yfact * yinc) <= EPD_1IN54_V2_HEIGHT - 1)
  //       yfact = yfact;
  //     if (ypos >= 0 && ypos + (yfact * yinc) > EPD_1IN54_V2_HEIGHT - 1)
  //       yfact = -1 * yfact;
  //     if (ypos < 0)
  //       yfact = -1 * yfact;
  //     // Update y-position
  //     ypos = ypos + (yfact * yinc);

  //     countdown = countdown - 1;
  //   }
  //   else if (lastTime)
  //   {
  //     EPD_1IN54_V2_Init();
  //     EPD_1IN54_V2_Clear();

  //     EPD_1IN54_V2_Sleep();
  //     free(BlackImage);
  //     BlackImage = NULL;

  //     lastTime = !(lastTime);
  //   }
  //   else
  //     sleep_ms(500);

  //   // flip LED state in next iteration
  //   ledval = ledval ^ 1;
  // }

  ws2812_deinit();

  return 0;
}
