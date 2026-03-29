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
// #define FONT_PALLET "ABCDEFGHIJKLMNOPQRSTUVWXYZ\rabcdefghijklmnopqrstuvwxyz\r0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
// #define FONT_PALLET "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
// #define FONT_PALLET "AaBbCcDdEeFfGgHhIiJjKkLlMm\rNnOoPpQqRrSsTtUuVvWwXxYyZz\r0123456789!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
#define FONT_PALLET "Aa\rBb\rCc\rDd\rEe\rFf\rGg\rHh\rIi\rJj\rKk\rLl\rMm\rNn\rOo\rPp\rQq\rRr\rSs\rTt\rUu\rVv\rWw\rXx\rYy\rZz\n0123456789\r!\"#$\%&'()*+,-./:;<=>?@[\\]^_`{|}~\b "
//        ^

#define WORD_SEPARATORS " \r\n\t&*+-/<=>\\_~"

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
#define BUTTON_IRQ_EVENT GPIO_IRQ_EDGE_RISE // GPIO_IRQ_EDGE_FALL

/////////////
// Display //
/////////////

#define EPAPER_MODEL "1in54v2"

#define EPAPER_SPI_PORT spi1
#define EPAPER_RST_PIN 12
#define EPAPER_DC_PIN 8
#define EPAPER_BUSY_PIN 13
#define EPAPER_CS_PIN 9
#define EPAPER_CLK_PIN 10
#define EPAPER_MOSI_PIN 11

#endif // CONFIG_H
