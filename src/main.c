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

#include "config.h"
#include "main.h"
#include "helpers.h"

#include <stdlib.h>       // itoa
#include "pico/bootrom.h" // rom_reset_usb_boot_extra
#include "pico/stdlib.h"  // sleep
#include "hardware/gpio.h"

#include "error.h"
#include "fatfs.h"
#include "usb.h"
#include "ws2812.h"
#include "epaper.h"
#include "gui.h"

#include "welcome.h"

FATFS fatfs_work_area;

button_action_t button_actions[4] = BUTTON_DEFAULT_ACTIONS;
absolute_time_t last_screen_update = {0};
absolute_time_t last_gpio_event_time[4] = {0};
bool button_pressed[4] = {false};

page_t page = PAGE_CATALOG;
uint8_t partial_refresh_count = PARTIAL_REFRESH_LIMIT + 1;
uint64_t scroll = 0;

char book[MAX_PATH_LENGTH] = "";
uint64_t book_scroll = 0;
int8_t book_scroll_requested = 0;

page_t history[HISTORY_LENGTH] = {PAGE_FONT_SIZE};
uint16_t history_index = 0;

uint16_t font_index = 5;
uint16_t fg_color = EPAPER_BLACK;
uint16_t bg_color = EPAPER_WHITE;

uint8_t *image_buffer;
bool screen_update_scheduled = true;
absolute_time_t now = {0};

char text_buffer_1[EPAPER_WIDTH * EPAPER_HEIGHT]; // Can store enough text to fill the entire screen with 1px font
char text_buffer_2[EPAPER_WIDTH * EPAPER_HEIGHT];

/**
 * \brief Attempt a screen update
 * \return true if the update was successful, false otherwise
 */
bool update_screen()
{
  if (epaper_is_busy())
    return false;

  uint16_t cursor_y = 0;

  switch (page)
  {
  case PAGE_CATALOG:
  {
    gui_draw_fill(EPAPER_WHITE);

    DIR root_directory;
    FILINFO file;
    FRESULT fatfs_result = f_opendir(&root_directory, "/");
    if (fatfs_result != FR_OK)
    {
      error(ERROR_FATFS_OPENDIR, false);
      return false;
    }

    cursor_y = gui_draw_string(0, cursor_y, "Welcome to mReader <3", &DEFAULT_FONT, fg_color, bg_color);

    for (uint8_t i = 0; i < 200; i++)
    {
      fatfs_result = f_readdir(&root_directory, &file); // Read a directory item

      if (fatfs_result != FR_OK)
      {
        error(ERROR_FATFS_READDIR, false);
        return false;
      }

      if (file.fname[0] == 0)
      {
        break;
      }

      if (!(file.fattrib & AM_DIR)) // If it's not a directory
      {
        if (i == scroll)
        {
          strlcpy(book, file.fname, sizeof(book));
          strlcpy(text_buffer_1, ">", sizeof(text_buffer_1));
        }
        else
          strlcpy(text_buffer_1, " ", sizeof(text_buffer_1));
        format_data_size(text_buffer_2, file.fsize, sizeof(text_buffer_2));
        strlcat(text_buffer_1, text_buffer_2, sizeof(text_buffer_1));
        strlcat(text_buffer_1, " ", sizeof(text_buffer_1));
        char file_name[256];
        strlcpy(file_name, file.fname, sizeof(file_name));
        strrchr(file_name, '.')[0] = 0; // Remove file extension for display
        strlcat(text_buffer_1, file_name, sizeof(text_buffer_1));

        cursor_y = gui_draw_string(0, cursor_y, text_buffer_1, &DEFAULT_FONT, fg_color, bg_color);
      }
      else if (i == scroll)
        scroll++; // Skip directories
    }

    FATFS *fs = {0};
    uint32_t free_clusters = 0;

    fatfs_result = f_getfree("", &free_clusters, &fs);
    if (fatfs_result != FR_OK)
    {
      error(ERROR_FATFS_GETFREE, false);
      return false;
    }

    cursor_y = gui_draw_string(0, cursor_y, "", &DEFAULT_FONT, fg_color, bg_color); // New line

    format_data_size(text_buffer_1, (fs->n_fatent - 2 - free_clusters) * fs->csize * DISK_BLOCK_SIZE, sizeof(text_buffer_1));
    strlcat(text_buffer_1, "/", sizeof(text_buffer_1));
    format_data_size(text_buffer_2, fs->n_fatent * fs->csize * DISK_BLOCK_SIZE, sizeof(text_buffer_2));
    strlcat(text_buffer_1, text_buffer_2, sizeof(text_buffer_1));
    strlcat(text_buffer_1, " Used. ", sizeof(text_buffer_1));
    format_data_size(text_buffer_2, free_clusters * fs->csize * DISK_BLOCK_SIZE, sizeof(text_buffer_2));
    strlcat(text_buffer_1, text_buffer_2, sizeof(text_buffer_1));
    strlcat(text_buffer_1, " Free", sizeof(text_buffer_1));

    cursor_y = gui_draw_string(0, cursor_y, text_buffer_1, &DEFAULT_FONT, fg_color, bg_color);

    fatfs_result = f_closedir(&root_directory);
    if (fatfs_result != FR_OK)
      error(ERROR_FATFS_CLOSEDIR, false);

    break;
  }

  case PAGE_READER:
  {
    bool success = true;

    FIL file;
    FRESULT fatfs_result = f_open(&file, book, FA_READ | FA_OPEN_EXISTING);
    if (fatfs_result != FR_OK)
    {
      flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
      sleep_ms(1000);
      error(ERROR_FATFS_OPEN, false);
      success = false;
    }
    else
    {
      if (book_scroll_requested > 0) // Scrolling forwards
      {
        success = read_file_at_offset(&file, book_scroll, text_buffer_1, sizeof(text_buffer_1));
        if (success)
        {
          char *newline = text_buffer_1;
          for (int s = 0; s < book_scroll_requested * SCROLL_SIZE; s++)
          {
            char *next_newline = strchr(newline, '\n');
            if (next_newline)
            {
              book_scroll += (next_newline - newline) + 1; // Move scroll by the distance to the newline
              newline = next_newline + 1;                  // Move past the newline for the next search
            }
            else
            {
              book_scroll += sizeof(text_buffer_1); // No newline, just move scroll by buffer size
              break;
            }
          }
          book_scroll_requested = 0;
        }
      }
      else if (book_scroll_requested < 0) // Scrolling backwards
      {
        uint32_t amount = (book_scroll < sizeof(text_buffer_1)) ? book_scroll : sizeof(text_buffer_1);
        uint64_t base = book_scroll - amount;

        success = read_file_at_offset(&file, base, text_buffer_1, amount);

        if (success)
        {
          uint32_t search_end = amount;

          for (uint32_t s = 0; s < -book_scroll_requested * SCROLL_SIZE; s++)
          {
            if (search_end > 0 && text_buffer_1[search_end - 1] == '\n')
            {
              search_end--;
            }

            char *newline = NULL;
            for (uint32_t i = search_end - 1; i >= 0; i--)
            {
              if (text_buffer_1[i] == '\n')
              {
                newline = &text_buffer_1[i];
                search_end = i + 1;
                break;
              }
            }

            if (newline)
            {
              book_scroll = base + (newline - text_buffer_1) + 1;
            }
            else
            {
              book_scroll = base;
              break;
            }
          }
          book_scroll_requested = 0;
        }
      }

      if (success)
      {
        success = read_file_at_offset(&file, book_scroll, text_buffer_1, sizeof(text_buffer_1));
      }
    }

    if (!success)
    {
      strlcpy(text_buffer_1, "Error reading '", sizeof(text_buffer_1));
      strlcat(text_buffer_1, book, sizeof(text_buffer_1));
      strlcat(text_buffer_1, "'", sizeof(text_buffer_1));
    }

    gui_draw_fill(EPAPER_WHITE);
    gui_draw_string(0, cursor_y, text_buffer_1, fonts[font_index], fg_color, bg_color);

    break;
  }

  case PAGE_FONT_SIZE:
  {
    itoa(fonts[font_index]->height, text_buffer_1, 10);
    strlcpy(text_buffer_2, "Font Size: ", sizeof(text_buffer_2));
    strlcat(text_buffer_2, text_buffer_1, sizeof(text_buffer_2));

    gui_draw_fill(EPAPER_WHITE);
    cursor_y = gui_draw_string(0, cursor_y, text_buffer_2, &DEFAULT_FONT, fg_color, bg_color);
    gui_draw_string(0, cursor_y, FONT_PALLET, fonts[font_index], fg_color, bg_color);

    break;
  }
  }

  partial_refresh_count++;

  if (partial_refresh_count > PARTIAL_REFRESH_LIMIT)
  {
    partial_refresh_count = 0;
    epaper_enter_full_mode();
  }

  epaper_display(image_buffer);

  if (partial_refresh_count == 0)
  {
    epaper_enter_partial_mode();
    epaper_display(image_buffer); // Upload current state
  }

  return true;
}

/**
 * \brief Run when a button is pressed
 */
void gpio_callback(const uint pin_number, const unsigned long events)
{
  if ((events & GPIO_IRQ_EDGE_RISE) && (events & GPIO_IRQ_EDGE_FALL))
  {
    return;
  }

  absolute_time_t now = get_absolute_time();

  // Map GPIO to button number with BUTTON_X_PIN
  button_action_t button_action = BUTTON_NEXT; // Default action, will be overridden
  switch (pin_number)
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

  if (absolute_time_diff_us(last_gpio_event_time[button_action], now) > DEBOUNCE_TIME_MS * 1000)
  {
    if (events & GPIO_IRQ_EDGE_RISE && !button_pressed[button_action])
    {
      button_pressed[button_action] = true;

      switch (page)
      {
      case PAGE_CATALOG:
        switch (button_action)
        {
        case BUTTON_NEXT:
          scroll++;
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          if (scroll > 0)
            scroll--;
          screen_update_scheduled = true;
          break;
        case BUTTON_ENTER:
          page = PAGE_READER;
          // partial_refresh_count = PARTIAL_REFRESH_LIMIT;
          history_index = (history_index + 1) % HISTORY_LENGTH;
          history[history_index] = PAGE_CATALOG;
          scroll = 0;
          screen_update_scheduled = true;
          break;
        case BUTTON_BACK:
          if (page != history[history_index])
            // partial_refresh_count = PARTIAL_REFRESH_LIMIT;
            page = history[history_index];
          history_index = (history_index - 1 + HISTORY_LENGTH) % HISTORY_LENGTH;
          screen_update_scheduled = true;
          break;
        }
        break;

      case PAGE_READER:
        switch (button_action)
        {
        case BUTTON_NEXT:
          book_scroll_requested++;
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          book_scroll_requested--;
          screen_update_scheduled = true;
          break;
        case BUTTON_ENTER:
          break;
        case BUTTON_BACK:
          if (page != history[history_index])
            // partial_refresh_count = PARTIAL_REFRESH_LIMIT;
            page = history[history_index];
          history_index = (history_index - 1 + HISTORY_LENGTH) % HISTORY_LENGTH;
          screen_update_scheduled = true;
          break;
        }
        break;

      case PAGE_FONT_SIZE:
        switch (button_action)
        {
        case BUTTON_NEXT:
          font_index = (font_index + 1) % FONT_COUNT; // Cycle through available fonts
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          font_index = (font_index - 1 + FONT_COUNT) % FONT_COUNT; // Cycle backwards through available fonts
          screen_update_scheduled = true;
          break;
        case BUTTON_ENTER:
          page = PAGE_CATALOG; // Sets history below
          // partial_refresh_count = PARTIAL_REFRESH_LIMIT;
          scroll = 0;
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
    else if (events & GPIO_IRQ_EDGE_FALL && button_pressed[button_action])
    {
      button_pressed[button_action] = false;
    }
  }

  last_gpio_event_time[button_action] = now;

  if (page == PAGE_CATALOG) // Back on the catalog page always goes to settings
  {
    history_index = 0;
    history[0] = PAGE_FONT_SIZE;
  }
}

int main()
{
  ws2812_init(LED_PIN);
  put_pixel(LED_OFF);
  sleep_ms(1); // Sometimes hangs without, idk

  // Register button interrupts
  gpio_init(BUTTON_0_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_0_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);
  gpio_init(BUTTON_1_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_1_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);
  gpio_init(BUTTON_2_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_2_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);
  gpio_init(BUTTON_3_PIN);
  gpio_set_irq_enabled_with_callback(BUTTON_3_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);

  epaper_init();
  image_buffer = gui_init(EPAPER_WIDTH, EPAPER_HEIGHT, DISPLAY_ROTATION, GUI_BITS_PER_PIXEL_2, GUI_MIRROR_NONE);
  gui_draw_fill(EPAPER_WHITE);

  // Mount and maybe format the flash
  if ((gpio_get(BUTTON_0_PIN) && gpio_get(BUTTON_1_PIN) && gpio_get(BUTTON_2_PIN) && gpio_get(BUTTON_3_PIN)) || !attempt_mount_flash(&fatfs_work_area, true))
  {
    put_pixel(LED_YELLOW);

    BYTE fatfs_format_work_area[FF_MAX_SS];
    FRESULT fatfs_result = f_mkfs("", NULL, fatfs_format_work_area, sizeof(fatfs_format_work_area));
    if (fatfs_result != FR_OK)
    {
      flash_code((char)fatfs_result, LED_BLUE, LED_GREEN, 6);
      sleep_ms(1000);
      error(ERROR_FATFS_MKFS, true);
      while (1)
      {
      }
    }

    put_pixel(LED_OFF);

    if (!attempt_mount_flash(&fatfs_work_area, false))
    {
      while (1)
      {
      }
    }

    FIL fil;
    f_open(&fil, "Welcome.md", FA_CREATE_NEW | FA_WRITE);
    f_write(&fil, welcome_file, sizeof(welcome_file) - 1, NULL);
    f_close(&fil);
  }

  usb_init();

  while (1)
  {
    /////////
    // USB //
    /////////

    usb_task();

    if (usb_just_unmounted)
    {
      usb_just_unmounted = false;
      if (page == PAGE_CATALOG)
      {
        scroll = 0;
        screen_update_scheduled = true;
      }
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
  }

  // Should be unreachable
  ws2812_deinit();
  return 0;
}
