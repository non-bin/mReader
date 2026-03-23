#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws2812/ws2812.h"
#include "epaper/epaper.h"

#define WS2812_PIN 16

#define RED    128, 0,   0
#define YELLOW 128, 128, 0
#define GREEN  0,   128, 0
#define CYAN   0,   128, 128
#define BLUE   0,   0,   128
#define PURPLE 64,  0,   128

int main() {
  ws2812_init(WS2812_PIN);

  while (1) {
    put_pixel(RED);
    sleep_ms(500);
    put_pixel(YELLOW);
    sleep_ms(500);
    put_pixel(GREEN);

    EPD_1in54_V2_test();

    sleep_ms(500);
    put_pixel(CYAN);
    sleep_ms(500);
    put_pixel(BLUE);
    sleep_ms(500);
    put_pixel(PURPLE);
    sleep_ms(500);
  }

  ws2812_deinit();
}
