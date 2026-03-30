/**
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright 2019 by the Waveshare team <https://github.com/waveshare/Pico_ePaper_Code>
 * \copyright 2022 by hammadrauf <https://github.com/hammadrauf/pico_epd_1in54>
 * \copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>
 */

#include "epaper.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "config.h"

#define EPAPER_LUT_LENGTH 159
typedef const uint8_t epaper_lut_t[EPAPER_LUT_LENGTH];

// waveform full refresh
static epaper_lut_t WAVEFORM_FULL_REFRESH =
    {
        0x80, 0x48, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x40, 0x48, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x80, 0x48, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x40, 0x48, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x8, 0x1, 0x0, 0x8, 0x1, 0x0, 0x2,
        0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0, 0x0, 0x0,
        0x22, 0x17, 0x41, 0x0, 0x32, 0x20};

// waveform partial refresh
static epaper_lut_t WAVEFORM_PARTIAL_REFRESH =
    {
        0x0, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80,
        0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x40, 0x40,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0, 0x0, 0x0, 0x02,
        0x17, 0x41, 0xB0, 0x32, 0x28};

/**
 * \brief Pulse the reset pin
 */
static void epaper_reset(void)
{
    gpio_put(EPAPER_RST_PIN, 1);
    sleep_ms(20); // FIXME should be async
    gpio_put(EPAPER_RST_PIN, 0);
    sleep_ms(2);
    gpio_put(EPAPER_RST_PIN, 1);
    sleep_ms(20);
}

/**
 * \brief Send a command byte
 */
static void epaper_send_command(const uint8_t reg)
{
    gpio_put(EPAPER_DC_PIN, 0);
    gpio_put(EPAPER_CS_PIN, 0);
    spi_write_blocking(EPAPER_SPI_PORT, &reg, 1);
    gpio_put(EPAPER_CS_PIN, 1);
}

/**
 * \brief Send a data byte
 */
static void epaper_send_data(const uint8_t data)
{
    gpio_put(EPAPER_DC_PIN, 1);
    gpio_put(EPAPER_CS_PIN, 0);
    spi_write_blocking(EPAPER_SPI_PORT, &data, 1);
    gpio_put(EPAPER_CS_PIN, 1);
}

/**
 * \brief Check if the display is busy
 * \return True if busy, false otherwise
 */
bool epaper_is_busy(void)
{
    return gpio_get(EPAPER_BUSY_PIN);
}

/**
 * \brief Blocking wait until the display is not busy
 * \warning BAD
 */
static void epaper_wait_busy(void)
{
    while (epaper_is_busy())
    {
        // FIXME Remove this
    }
}

/**
 * \brief Signal the display to refresh
 * \param partial Whether the display is in partial mode. Must be True when using partial refresh
 */
static void epaper_turn_on_display(const bool partial)
{
    epaper_send_command(0x22);
    epaper_send_data(partial ? 0xcF : 0xc7);
    epaper_send_command(0x20);
    epaper_wait_busy();
}

/**
 * \brief Send a new LUT to the display
 * \param lut Pointer to the LUT array
 */
static void epaper_send_look_up_table(epaper_lut_t lut)
{
    // Send LUT
    epaper_send_command(0x32);
    for (uint8_t i = 0; i < EPAPER_LUT_LENGTH - 6; i++)
        epaper_send_data(lut[i]);
    epaper_wait_busy();

    // Select LUT
    epaper_send_command(0x3f);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 6]);

    epaper_send_command(0x03);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 5]);

    epaper_send_command(0x04);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 4]);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 3]);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 2]);

    epaper_send_command(0x2c);
    epaper_send_data(lut[EPAPER_LUT_LENGTH - 1]);
}

/**
 * \brief Tell the display to only refresh within the area from (x_start, y_start) to (x_end, y_end)
 */
static void epaper_set_windows(const uint16_t x_start, const uint16_t y_start, const uint16_t x_end, const uint16_t y_end)
{
    epaper_send_command(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    epaper_send_data((x_start >> 3) & 0xFF);
    epaper_send_data((x_end >> 3) & 0xFF);

    epaper_send_command(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    epaper_send_data(y_start & 0xFF);
    epaper_send_data((y_start >> 8) & 0xFF);
    epaper_send_data(y_end & 0xFF);
    epaper_send_data((y_end >> 8) & 0xFF);
}

/**
 * \brief TODO idk
 */
static void epaper_set_cursor(const uint16_t x_start, const uint16_t y_start)
{
    epaper_send_command(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    epaper_send_data(x_start & 0xFF);

    epaper_send_command(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    epaper_send_data(y_start & 0xFF);
    epaper_send_data((y_start >> 8) & 0xFF);
}

/**
 * \brief Initialise a gpio pin and set it's direction
 */
static void epaper_set_gpio_mode(const uint16_t pin, const enum gpio_dir mode)
{
    gpio_init(pin);
    if (mode == GPIO_IN)
    {
        gpio_set_dir(pin, GPIO_IN);
    }
    else
    {
        gpio_set_dir(pin, GPIO_OUT);
    }
}

/**
 * \brief Setup the epaper display
 */
void epaper_init(void)
{
    stdio_init_all();

    // GPIO Config
    epaper_set_gpio_mode(EPAPER_RST_PIN, GPIO_OUT);
    epaper_set_gpio_mode(EPAPER_DC_PIN, GPIO_OUT);
    epaper_set_gpio_mode(EPAPER_CS_PIN, GPIO_OUT);
    epaper_set_gpio_mode(EPAPER_BUSY_PIN, GPIO_IN);
    gpio_put(EPAPER_CS_PIN, 1);

    // Initialize SPI port at 1 MHz
    spi_init(EPAPER_SPI_PORT, 1000 * 1000);

    // Set SPI format
    spi_set_format(EPAPER_SPI_PORT, // SPI instance
                   8,               // Number of bits per transfer
                   0,               // Polarity (CPOL)
                   0,               // Phase (CPHA)
                   SPI_MSB_FIRST);

    gpio_set_function(EPAPER_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(EPAPER_MOSI_PIN, GPIO_FUNC_SPI);

    epaper_reset();

    epaper_wait_busy();
    epaper_send_command(0x12); // SWRESET
    epaper_wait_busy();

    epaper_send_command(0x01); // Driver output control
    epaper_send_data(0xC7);
    epaper_send_data(0x00);
    epaper_send_data(0x01);

    epaper_send_command(0x11); // data entry mode
    epaper_send_data(0x01);

    epaper_set_windows(0, EPAPER_HEIGHT - 1, EPAPER_WIDTH - 1, 0);

    epaper_send_command(0x3C); // BorderWavefrom
    epaper_send_data(0x01);

    epaper_send_command(0x18);
    epaper_send_data(0x80);

    epaper_send_command(0x22); // //Load Temperature and waveform setting.
    epaper_send_data(0XB1);
    epaper_send_command(0x20);

    epaper_set_cursor(0, -1);
    epaper_wait_busy();

    epaper_send_look_up_table(WAVEFORM_FULL_REFRESH);
}

/**
 * \brief Enter partial display mode
 */
void epaper_partial_init(void)
{
    epaper_reset();
    epaper_wait_busy();

    epaper_send_look_up_table(WAVEFORM_PARTIAL_REFRESH);
    epaper_send_command(0x37);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x40);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x00);
    epaper_send_data(0x00);

    epaper_send_command(0x3C); // BorderWavefrom
    epaper_send_data(0x80);

    epaper_send_command(0x22);
    epaper_send_data(0xc0);
    epaper_send_command(0x20);
    epaper_wait_busy();
}

/**
 * \brief Immediately fill the display with the specified color and refresh
 */
void epaper_clear(const uint16_t color)
{
    epaper_send_command(0x24);
    for (uint16_t j = 0; j < EPAPER_HEIGHT; j++)
    {
        for (uint16_t i = 0; i < EPAPER_WIDTH_BYTES; i++)
        {
            epaper_send_data(color);
        }
    }
    epaper_send_command(0x26);
    for (uint16_t j = 0; j < EPAPER_HEIGHT; j++)
    {
        for (uint16_t i = 0; i < EPAPER_WIDTH_BYTES; i++)
        {
            epaper_send_data(color);
        }
    }
    epaper_turn_on_display(false);
}

/**
 * \brief Send an image buffer to the display, and display it
 * \param partial Whether the display is in partial mode. Must be True when using partial refresh
 */
void epaper_display(const uint8_t *image, const bool partial)
{
    uint32_t address = 0;
    epaper_send_command(0x24);

    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_WIDTH_BYTES; j++)
        {
            address = j + i * EPAPER_WIDTH_BYTES;
            epaper_send_data(image[address]);
        }
    }

    epaper_turn_on_display(partial);
}

/**
 * \brief Upload the image of the previous frame, otherwise the first few seconds will display an exception
 */
void epaper_partial_upload_base_image(const uint8_t *image)
{
    uint32_t address = 0;
    epaper_send_command(0x24);
    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_WIDTH_BYTES; j++)
        {
            address = j + i * EPAPER_WIDTH_BYTES;
            epaper_send_data(image[address]);
        }
    }
    epaper_send_command(0x26);
    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_WIDTH_BYTES; j++)
        {
            address = j + i * EPAPER_WIDTH_BYTES;
            epaper_send_data(image[address]);
        }
    }
    epaper_turn_on_display(true);
}

/**
 * \brief Enter sleep mode
 */
void epaper_sleep(void)
{
    epaper_send_command(0x10); // enter deep sleep
    epaper_send_data(0x01);
}
