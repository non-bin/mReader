#ifndef CONFIG_H
#define CONFIG_H

// Settings

#define FONT_SIZE 8

#define WS2812_PIN 16

#define BUTTON_1_PIN 0
#define BUTTON_2_PIN 2
#define BUTTON_3_PIN 4
#define BUTTON_4_PIN 6
#define DEBOUNCE_TIME_MS 20

#define SPI_PORT spi1
#define EPD_RST_PIN 12
#define EPD_DC_PIN 8
#define EPD_BUSY_PIN 13
#define EPD_CS_PIN 9
#define EPD_CLK_PIN 10
#define EPD_MOSI_PIN 11

// Logic

#if FONT_SIZE == 24
#define FONT Font24
#elif FONT_SIZE == 20
#define FONT Font20
#elif FONT_SIZE == 16
#define FONT Font16
#elif FONT_SIZE == 12
#define FONT Font12
#elif FONT_SIZE == 8
#define FONT Font8
#else
#error "Unsupported FONT_SIZE"
#endif

#endif // CONFIG_H
