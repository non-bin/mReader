#include "config.h"

#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"

#include "tusb.h"
#include "msc_disk.h"
#include "ws2812.h"
#include "epaper.h"
#include "GUI_Paint.h"

absolute_time_t last_screen_update = {0};
absolute_time_t last_gpio_event_time[8] = {0};
bool button_pressed[8] = {false};

typedef enum
{
  BUTTON_NEXT,
  BUTTON_PREVIOUS,
  BUTTON_ENTER,
  BUTTON_BACK
} button_action_t;

button_action_t button_actions[4] = {BUTTON_NEXT, BUTTON_ENTER, BUTTON_BACK, BUTTON_PREVIOUS};

typedef enum
{
  PAGE_CATALOG,
  PAGE_READER,
  PAGE_FONT_SIZE
} page_t;

typedef struct
{
  uint64_t scroll;
  page_t current_page;
  page_t history[HISTORY_LENGTH];
  uint16_t history_index;
  uint16_t font_index;
  uint16_t fg_color;
  uint16_t bg_color;
} state_t;

state_t state = {
    .scroll = 0,
    .current_page = PAGE_CATALOG,
    .history = {PAGE_FONT_SIZE},
    .history_index = 0,
    .font_index = 5,
    .fg_color = EPAPER_BLACK,
    .bg_color = EPAPER_WHITE,
};

uint8_t *image_buffer;
bool screen_update_scheduled = true;
absolute_time_t now = {0};

char text_buffer_1[EPAPER_IMAGE_SIZE]; // Can store enough text to fill the entire screen with 1px font
char text_buffer_2[EPAPER_IMAGE_SIZE];

extern volatile bool g_usb_just_unmounted;
extern volatile bool g_usb_mounted;

void read_book(uint64_t offset)
{
  msc_file_info_t files[10];
  int count = msc_disk_get_file_list(files, 10);

  if (count > 0)
  {
    int read_len = msc_disk_read_file(files[0].start_cluster, files[0].size, (uint8_t *)text_buffer_1, sizeof(text_buffer_1) - 1, offset);
    text_buffer_1[read_len] = '\0';
  }
  return;
}

bool update_screen()
{
  if (epaper_is_busy())
    return false;

  epaper_draw_fill(EPAPER_WHITE);

  uint16_t cursor_y = 0;

  switch (state.current_page)
  {
  case PAGE_CATALOG:
    epaper_draw_string(0, cursor_y, "Welcome to mReader <3", &DEFAULT_FONT, state.fg_color, state.bg_color);
    epaper_display(image_buffer, false);
    break;
  case PAGE_READER:
    read_book(state.scroll);
    epaper_draw_string(0, cursor_y, text_buffer_1, fonts[state.font_index], state.fg_color, state.bg_color);
    epaper_display(image_buffer, false);
    break;
  case PAGE_FONT_SIZE:
    itoa(fonts[state.font_index]->Height, text_buffer_1, 10);
    strlcpy(text_buffer_2, "Font Size: ", sizeof(text_buffer_2));
    strlcat(text_buffer_2, text_buffer_1, sizeof(text_buffer_2));

    cursor_y = epaper_draw_string(0, cursor_y, text_buffer_2, &DEFAULT_FONT, state.fg_color, state.bg_color);
    epaper_draw_string(0, cursor_y, FONT_PALLET, fonts[state.font_index], state.fg_color, state.bg_color);

    epaper_display(image_buffer, false);
    break;
  }

  return true;
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

      // Map GPIO to button number with BUTTON_X_PIN
      button_action_t button_action = BUTTON_NEXT; // Default action, will be overridden
      switch (gpio)
      {
      case BUTTON_0_PIN:
        button_action = button_actions[0];
        break;
      case BUTTON_1_PIN:
        button_action = button_actions[1];
        break;
      case BUTTON_2_PIN:
        button_action = button_actions[2];
        break;
      case BUTTON_3_PIN:
        button_action = button_actions[3];
        break;
      default:
        return; // Ignore unrecognized GPIOs
      }

      switch (state.current_page)
      {
      case PAGE_CATALOG:
        switch (button_action)
        {
        case BUTTON_NEXT:
          break;
        case BUTTON_PREVIOUS:
          break;
        case BUTTON_ENTER:
          state.current_page = PAGE_READER;
          state.history_index = (state.history_index + 1) % HISTORY_LENGTH;
          state.history[state.history_index] = PAGE_CATALOG;
          state.scroll = 0;
          screen_update_scheduled = true;
          break;
        case BUTTON_BACK:
          state.current_page = state.history[state.history_index];
          state.history_index = (state.history_index - 1 + HISTORY_LENGTH) % HISTORY_LENGTH;
          screen_update_scheduled = true;
          break;
        }
        break;
      case PAGE_READER:
        switch (button_action)
        {
        case BUTTON_NEXT:
          state.scroll += SCROLL_SIZE;
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          if (state.scroll >= SCROLL_SIZE)
          {
            state.scroll -= SCROLL_SIZE;
            screen_update_scheduled = true;
          }
          break;
        case BUTTON_ENTER:
          break;
        case BUTTON_BACK:
          state.current_page = state.history[state.history_index];
          state.history_index = (state.history_index - 1 + HISTORY_LENGTH) % HISTORY_LENGTH;
          screen_update_scheduled = true;
          break;
        }
        break;
      case PAGE_FONT_SIZE:
        switch (button_action)
        {
        case BUTTON_NEXT:
          state.font_index = (state.font_index + 1) % FONT_COUNT; // Cycle through available fonts
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          state.font_index = (state.font_index - 1 + FONT_COUNT) % FONT_COUNT; // Cycle backwards through available fonts
          screen_update_scheduled = true;
          break;
        case BUTTON_ENTER:
          state.current_page = PAGE_CATALOG; // Sets history below
          state.scroll = 0;
          screen_update_scheduled = true;
          break;
        case BUTTON_BACK:
          put_pixel(LED_PURPLE);
          rom_reset_usb_boot_extra(-1, 0, 0); // Reset to BOOTSEL
          break;
        }
        break;
      }
    }
    else if (events & GPIO_IRQ_EDGE_FALL && button_pressed[gpio])
    {
      button_pressed[gpio] = false;
    }
  }

  last_gpio_event_time[gpio] = now;

  if (state.current_page == PAGE_CATALOG) // Back on the catalog page always goes to settings
  {
    state.history_index = 0;
    state.history[0] = PAGE_FONT_SIZE;
  }
}

int main()
{
  // Initialize WS2812 LED
  ws2812_init(WS2812_PIN);
  put_pixel(LED_OFF);

  msc_disk_lazy_format_fat16(); // Ensure filesystem exists
  tusb_init();                  // Initialize TinyUSB

  // Register button interrupts
  gpio_init(BUTTON_0_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_0_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_1_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_1_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_2_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_2_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_init(BUTTON_3_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_3_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

  epaper_init();
  image_buffer = epaper_create_image_buffer(EPAPER_WIDTH, EPAPER_HEIGHT, 270, EPAPER_WHITE);
  epaper_select_image_buffer(image_buffer);

  uint16_t color = 1;

  while (1)
  {
    /////////
    // USB //
    /////////

    tud_task(); // TinyUSB device task

    if (g_usb_just_unmounted)
    {
      g_usb_just_unmounted = false;
      // text = read_book();
    }

    ////////////
    // Screen //
    ////////////

    if (screen_update_scheduled)
    {
      screen_update_scheduled = false;
      if (!update_screen())
        screen_update_scheduled = true; // Schedule another if it failed
    }

    //   epaper_draw_fill(state.bg_color);
    // epaper_draw_number(0, 0, offset, &DEFAULT_FONT, state.fg_color, state.bg_color);
    //   epaper_draw_string(0, 4, text + offset, &DEFAULT_FONT, state.fg_color, state.bg_color);

    //   epaper_display(imageCache, false);
  }

  // Should be unreachable
  ws2812_deinit();
  return 0;
}
