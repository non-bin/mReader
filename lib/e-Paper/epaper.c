#include <stdlib.h>
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "EPD_1in54_V2.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ws2812.h"

void set_GPIO_mode(uint16_t Pin, uint16_t Mode)
{
  gpio_init(Pin);
  if (Mode == 0 || Mode == GPIO_IN)
  {
    gpio_set_dir(Pin, GPIO_IN);
  }
  else
  {
    gpio_set_dir(Pin, GPIO_OUT);
  }
}

void epaper_GPIO_init(void)
{
  set_GPIO_mode(EPD_RST_PIN, 1);
  set_GPIO_mode(EPD_DC_PIN, 1);
  set_GPIO_mode(EPD_CS_PIN, 1);
  set_GPIO_mode(EPD_BUSY_PIN, 0);

  gpio_put(EPD_CS_PIN, 1);
}

uint8_t epaper_init(void)
{
  stdio_init_all();

  // GPIO Config
  epaper_GPIO_init();

  // Initialize SPI port at 1 MHz
  spi_init(SPI_PORT, 1000 * 1000);

  // Set SPI format
  spi_set_format(SPI_PORT, // SPI instance
                 8,        // Number of bits per transfer
                 0,        // Polarity (CPOL)
                 0,        // Phase (CPHA)
                 SPI_MSB_FIRST);

  gpio_set_function(EPD_CLK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(EPD_MOSI_PIN, GPIO_FUNC_SPI);

  EPD_1IN54_V2_Init();

  return 0;
}

void epaper_clear(void)
{
  EPD_1IN54_V2_Clear();
}

uint8_t *epaper_create_image_buffer(void)
{
  uint8_t *imageBuffer = (uint8_t *)malloc(EPD_1IN54_V2_IMAGESIZE);
  Paint_NewImage(imageBuffer, EPD_1IN54_V2_WIDTH, EPD_1IN54_V2_HEIGHT, 270, WHITE);

  return imageBuffer;
}

void epaper_select_image_buffer(uint8_t *imageBuffer)
{
  Paint_SelectImage(imageBuffer);
}

void epaper_draw_bitmap(uint8_t *imageBuffer, const uint8_t *bitmap)
{
  Paint_DrawBitMap(bitmap);
}

void epaper_fill(uint16_t Color)
{
  Paint_Clear(Color);
}

void epaper_display(uint8_t *imageBuffer)
{
  EPD_1IN54_V2_Display(imageBuffer);
}

uint16_t epaper_draw_number(uint16_t Xpoint, uint16_t Ypoint, int32_t Nummber, font_t *Font, uint16_t Color_Foreground, uint16_t Color_Background)
{
  return Paint_DrawNum(Xpoint, Ypoint, Nummber, Font, Color_Foreground, Color_Background);
}

uint16_t epaper_draw_string(uint16_t Xstart, uint16_t Ystart, const char *pString, font_t *Font, uint16_t Color_Foreground, uint16_t Color_Background)
{
  return Paint_DrawString_EN(Xstart, Ystart, pString, Font, Color_Foreground, Color_Background);
}

bool epaper_is_busy(void)
{
  return EPD_1IN54_V2_read_busy();
}

void epaper_wait_busy(void)
{
  EPD_1IN54_V2_wait_busy();
}
