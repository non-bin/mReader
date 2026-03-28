#include "config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "ws2812.h"
#include "epaper.h"

#define EPAPER_ADJUSTED_WIDTH (EPAPER_WIDTH % 8 == 0) ? (EPAPER_WIDTH / 8) : (EPAPER_WIDTH / 8 + 1)

// waveform full refresh
static const unsigned char WAVEFORM_FULL_REFRESH[159] =
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

// waveform partial refresh(fast)
static const unsigned char WAVEFORM_PARTIAL_REFRESH[159] =
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

static void epaper_reset(void)
{
    gpio_put(EPAPER_RST_PIN, 1);
    sleep_ms(20); // FIXME should be async
    gpio_put(EPAPER_RST_PIN, 0);
    sleep_ms(2);
    gpio_put(EPAPER_RST_PIN, 1);
    sleep_ms(20);
}

static void epaper_send_command(uint8_t reg)
{
    gpio_put(EPAPER_DC_PIN, 0);
    gpio_put(EPAPER_CS_PIN, 0);
    spi_write_blocking(EPAPER_SPI_PORT, &reg, 1);
    gpio_put(EPAPER_CS_PIN, 1);
}

static void epaper_send_data(uint8_t data)
{
    gpio_put(EPAPER_DC_PIN, 1);
    gpio_put(EPAPER_CS_PIN, 0);
    spi_write_blocking(EPAPER_SPI_PORT, &data, 1);
    gpio_put(EPAPER_CS_PIN, 1);
}

bool epaper_is_busy(void)
{
    return gpio_get(EPAPER_BUSY_PIN);
}

static void epaper_wait_busy(void)
{
    while (epaper_is_busy())
    {
        // FIXME Remove this
    }
}

// Partial must be True when using partial refresh
static void epaper_turn_on_display(bool partial)
{
    epaper_send_command(0x22);
    epaper_send_data(partial ? 0xcF : 0xc7);
    epaper_send_command(0x20);
    epaper_wait_busy();
}

static void epaper_send_look_up_table(const uint8_t *lut)
{
    // Send LUT
    epaper_send_command(0x32);
    for (uint8_t i = 0; i < 153; i++)
        epaper_send_data(lut[i]);
    epaper_wait_busy();

    // Select LUT
    epaper_send_command(0x3f);
    epaper_send_data(lut[153]);

    epaper_send_command(0x03);
    epaper_send_data(lut[154]);

    epaper_send_command(0x04);
    epaper_send_data(lut[155]);
    epaper_send_data(lut[156]);
    epaper_send_data(lut[157]);

    epaper_send_command(0x2c);
    epaper_send_data(lut[158]);
}

static void epaper_set_windows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
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

static void epaper_set_cursor(uint16_t x_start, uint16_t y_start)
{
    epaper_send_command(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    epaper_send_data(x_start & 0xFF);

    epaper_send_command(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    epaper_send_data(y_start & 0xFF);
    epaper_send_data((y_start >> 8) & 0xFF);
}

static void epaper_set_gpio_mode(uint16_t pin, enum gpio_dir mode)
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

// Enter partial display mode
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

void epaper_clear(uint16_t color)
{
    epaper_send_command(0x24);
    for (uint16_t j = 0; j < EPAPER_HEIGHT; j++)
    {
        for (uint16_t i = 0; i < EPAPER_ADJUSTED_WIDTH; i++)
        {
            epaper_send_data(color);
        }
    }
    epaper_send_command(0x26);
    for (uint16_t j = 0; j < EPAPER_HEIGHT; j++)
    {
        for (uint16_t i = 0; i < EPAPER_ADJUSTED_WIDTH; i++)
        {
            epaper_send_data(color);
        }
    }
    epaper_turn_on_display(false);
}

// Send an image buffer and display it. Partial must be True when using partial refresh
void epaper_partial_display(uint8_t *image, bool partial)
{
    uint32_t address = 0;
    epaper_send_command(0x24);
    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_ADJUSTED_WIDTH; j++)
        {
            address = j + i * EPAPER_ADJUSTED_WIDTH;
            epaper_send_data(image[address]);
        }
    }
    epaper_turn_on_display(partial);
}

// The image of the previous frame must be uploaded, otherwise the first few seconds will display an exception.
void epaper_partial_upload_base_image(uint8_t *image)
{
    uint32_t address = 0;
    epaper_send_command(0x24);
    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_ADJUSTED_WIDTH; j++)
        {
            address = j + i * EPAPER_ADJUSTED_WIDTH;
            epaper_send_data(image[address]);
        }
    }
    epaper_send_command(0x26);
    for (uint16_t i = 0; i < EPAPER_HEIGHT; i++)
    {
        for (uint16_t j = 0; j < EPAPER_ADJUSTED_WIDTH; j++)
        {
            address = j + i * EPAPER_ADJUSTED_WIDTH;
            epaper_send_data(image[address]);
        }
    }
    epaper_turn_on_display(true);
}

// Enter sleep mode
void epaper_sleep(void)
{
    epaper_send_command(0x10); // enter deep sleep
    epaper_send_data(0x01);
}
