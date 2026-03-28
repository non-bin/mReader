#ifndef CONFIG_H
#define CONFIG_H

///////////
// Debug //
///////////

// #define ANGEL_KISSES

/////////////
// General //
/////////////

#define HISTORY_LENGTH 4
#define SCROLL_SIZE 100

//////////
// Text //
//////////

#define DEFAULT_FONT font12
#define TAB_SIZE 2 // tabWidth = (space.width + characterGap) * TAB_SIZE

// Displayed to demo each font size
// #define FONT_PALLET " !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\b"
// #define FONT_PALLET "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
// #define FONT_PALLET "AaBbCcDdEeFfGgHhIiJjKkLlMm\nNnOoPpQqRrSsTtUuVvWwXxYyZz\n0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
#define FONT_PALLET "AaBbCcDdEeFfGgHhIi\nJjKkLlMmNnOoPpQqRr\nSsTtUuVvWwXxYyZz\n0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
#define WORD_SEPARATORS " \n\t&*+-/<=>\\_~"

// #define FONT3_WIDE_M
// #define FONT3_WIDE_N
// #define FONT3_WIDE_Q
// #define FONT3_WIDE_W

// #define FONT4_WIDE_UP_M
#define FONT4_WIDE_UP_N
// #define FONT4_WIDE_UP_W
// #define FONT4_WIDE_LO_M
// #define FONT4_WIDE_LO_W

#define FONT5_WIDE_HASH
#define FONT5_WIDE_AMPERSAND
#define FONT5_WIDE_AT
// #define FONT5_WIDE_UP_M
#define FONT5_WIDE_UP_N

#define FONT6_WIDE_UP_M
#define FONT6_WIDE_UP_N
#define FONT6_WIDE_UP_Q
#define FONT6_WIDE_UP_W
#define FONT6_TALL_LO_G
// #define FONT6_TALL_LO_I
#define FONT6_WIDE_LO_W

/////////
// LED //
/////////

#define WS2812_PIN 16

/////////////
// Buttons //
/////////////

#define BUTTON_0_PIN 0
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 4
#define BUTTON_3_PIN 6
#define DEBOUNCE_TIME_MS 20

/////////////
// Display //
/////////////

#define SPI_PORT spi1
#define EPD_RST_PIN 12
#define EPD_DC_PIN 8
#define EPD_BUSY_PIN 13
#define EPD_CS_PIN 9
#define EPD_CLK_PIN 10
#define EPD_MOSI_PIN 11

#endif // CONFIG_H
