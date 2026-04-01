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

#include <stdlib.h>       // itoa
#include "pico/bootrom.h" // rom_reset_usb_boot_extra
#include "hardware/gpio.h"

#include "error.h"
#include "ff.h"
#include "tusb.h"
#include "tinyusb.h"
#include "msc_disk.h"
#include "ws2812.h"
#include "epaper.h"
#include "gui.h"

FATFS fatfs_work_area;

button_action_t button_actions[4] = BUTTON_DEFAULT_ACTIONS;
absolute_time_t last_screen_update = {0};
absolute_time_t last_gpio_event_time[4] = {0};
bool button_pressed[4] = {false};

state_t state = {
    .scroll = 0,
    .current_book = "",
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

  gui_draw_fill(EPAPER_WHITE);

  uint16_t cursor_y = 0;

  switch (state.current_page)
  {
  case PAGE_CATALOG:
  {
    DIR root_directory;
    FILINFO file;
    FRESULT fatfs_result = f_opendir(&root_directory, "/");
    if (fatfs_result != FR_OK)
    {
      error(ERROR_FATFS_OPENDIR, false);
      return false;
    }

    cursor_y = gui_draw_string(0, cursor_y, "Welcome to mReader <3", &DEFAULT_FONT, state.fg_color, state.bg_color);

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
        if (i == state.scroll)
        {
          strlcpy(state.current_book, file.fname, sizeof(state.current_book));
          strlcpy(text_buffer_1, "> ", sizeof(text_buffer_1));
        }
        else
          strlcpy(text_buffer_1, "  ", sizeof(text_buffer_1));
        itoa((int)file.fsize, text_buffer_2, 10);
        strlcat(text_buffer_1, text_buffer_2, sizeof(text_buffer_1));
        strlcat(text_buffer_1, "b ", sizeof(text_buffer_1));
        strlcat(text_buffer_1, file.fname, sizeof(text_buffer_1));

        cursor_y = gui_draw_string(0, cursor_y, text_buffer_1, &DEFAULT_FONT, state.fg_color, state.bg_color);
      }
    }

    fatfs_result = f_closedir(&root_directory);
    if (fatfs_result != FR_OK)
      error(ERROR_FATFS_CLOSEDIR, false);

    epaper_display(image_buffer, false);
    return true;
  }

  case PAGE_READER:
  {
    bool success = true;

    FIL file;
    FRESULT fatfs_result = f_open(&file, state.current_book, FA_READ | FA_OPEN_EXISTING);
    if (fatfs_result != FR_OK)
    {
      flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
      sleep_ms(1000);
      error(ERROR_FATFS_OPEN, false);
      success = false;
    }
    else
    {
      fatfs_result = f_lseek(&file, state.scroll);
      if (fatfs_result != FR_OK)
      {
        flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
        sleep_ms(1000);
        error(ERROR_FATFS_LSEEK, false);
        success = false;
      }
      else
      {
        fatfs_result = f_read(&file, text_buffer_1, sizeof(text_buffer_1), NULL);
        if (fatfs_result != FR_OK)
        {
          flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
          sleep_ms(1000);
          error(ERROR_FATFS_LSEEK, false);
          success = false;
        }
      }
    }

    if (!success)
    {
      strlcpy(text_buffer_1, "Error reading '", sizeof(text_buffer_1));
      strlcat(text_buffer_1, state.current_book, sizeof(text_buffer_1));
      strlcat(text_buffer_1, "'", sizeof(text_buffer_1));
    }

    gui_draw_string(0, cursor_y, text_buffer_1, fonts[state.font_index], state.fg_color, state.bg_color);
    epaper_display(image_buffer, false);
    return success;
  }

  case PAGE_FONT_SIZE:
  {
    itoa(fonts[state.font_index]->height, text_buffer_1, 10);
    strlcpy(text_buffer_2, "Font Size: ", sizeof(text_buffer_2));
    strlcat(text_buffer_2, text_buffer_1, sizeof(text_buffer_2));

    cursor_y = gui_draw_string(0, cursor_y, text_buffer_2, &DEFAULT_FONT, state.fg_color, state.bg_color);
    gui_draw_string(0, cursor_y, FONT_PALLET, fonts[state.font_index], state.fg_color, state.bg_color);

    epaper_display(image_buffer, false);
    return true;
  }
  }

  return false;
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

      switch (state.current_page)
      {
      case PAGE_CATALOG:
        switch (button_action)
        {
        case BUTTON_NEXT:
          state.scroll++;
          screen_update_scheduled = true;
          break;
        case BUTTON_PREVIOUS:
          if (state.scroll > 0)
            state.scroll--;
          screen_update_scheduled = true;
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
    else if (events & GPIO_IRQ_EDGE_FALL && button_pressed[button_action])
    {
      button_pressed[button_action] = false;
    }
  }

  last_gpio_event_time[button_action] = now;

  if (state.current_page == PAGE_CATALOG) // Back on the catalog page always goes to settings
  {
    state.history_index = 0;
    state.history[0] = PAGE_FONT_SIZE;
  }
}

/**
 * \brief Try to mount the filesystem
 * \param fail_gracefully Whether to continue on error (true) or halt (false)
 * \return True if the drive was successfully mounted, false otherwise
 */
bool attempt_mount_flash(bool fail_gracefully)
{
  FRESULT fatfs_result = f_mount(&fatfs_work_area, "", 0);
  if (fatfs_result == FR_NO_FILESYSTEM)
    return false;
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
    sleep_ms(1000);
    error(ERROR_FATFS_MOUNT, true);
    while (1)
    {
    }
  }

  fatfs_result = f_setlabel("mReader");
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
    sleep_ms(1000);
    error(ERROR_FATFS_SETLABEL, false);
  }

  DIR root_directory;
  fatfs_result = f_opendir(&root_directory, "/");
  if (fatfs_result == FR_NO_FILESYSTEM)
    return false;
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
    sleep_ms(1000);
    error(ERROR_FATFS_OPENDIR, true);
    return false;
  }

  FILINFO file;
  fatfs_result = f_readdir(&root_directory, &file);
  if (fatfs_result != FR_OK)
  {
    flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
    sleep_ms(1000);
    error(ERROR_FATFS_READDIR, fail_gracefully);
    return false;
  }

  fatfs_result = f_closedir(&root_directory);
  if (fatfs_result != FR_OK)
    error(ERROR_FATFS_CLOSEDIR, fail_gracefully);

  return true;
}

int main()
{
  ws2812_init(LED_PIN);
  put_pixel(LED_OFF);

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
  image_buffer = gui_init(EPAPER_WIDTH, EPAPER_HEIGHT, GUI_ROTATE_270, GUI_BITS_PER_PIXEL_2, GUI_MIRROR_NONE);

  // Mount and maybe format the flash
  if ((gpio_get(BUTTON_0_PIN) && gpio_get(BUTTON_1_PIN) && gpio_get(BUTTON_2_PIN) && gpio_get(BUTTON_3_PIN)) || !attempt_mount_flash(true))
  {
    put_pixel(LED_YELLOW);

    BYTE fatfs_format_work_area[FF_MAX_SS];
    FRESULT fatfs_result = f_mkfs("", NULL, fatfs_format_work_area, sizeof(fatfs_format_work_area));
    if (fatfs_result != FR_OK)
    {
      flash_code((char)fatfs_result, LED_BLUE, LED_GREEN);
      sleep_ms(1000);
      error(ERROR_FATFS_MKFS, true);
      while (1)
      {
      }
    }

    put_pixel(LED_OFF);

    if (!attempt_mount_flash(false))
    {
      while (1)
      {
      }
    }

    FIL fil;
    f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    f_write(&fil, "Hello, World!\n", 15, NULL);
    f_close(&fil);
    f_open(&fil, "book.txt", FA_CREATE_NEW | FA_WRITE);
#define TEST_STRING "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible.\n"
    f_write(&fil, TEST_STRING, sizeof(TEST_STRING), NULL);
    f_close(&fil);
  }

  while (1)
  {
    /////////
    // USB //
    /////////

    // tud_task(); // TinyUSB device task

    if (usb_just_unmounted)
    {
      usb_just_unmounted = false;
      // text = read_book();
    }

    ////////////
    // Screen //
    ////////////

    // sleep_ms(1);
    // put_pixel(LED_RED);
    // sleep_ms(1);

    if (screen_update_scheduled)
    {
      screen_update_scheduled = false;
      if (!update_screen())
        screen_update_scheduled = true; // Schedule another if it failed
    }

    sleep_ms(1);
    // put_pixel(LED_GREEN);
    // sleep_ms(1);
  }

  // Should be unreachable
  ws2812_deinit();
  return 0;
}
