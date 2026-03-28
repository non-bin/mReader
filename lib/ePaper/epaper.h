#ifndef _EPAPER_H_
#define _EPAPER_H_

#include "config.h"
#include "../Fonts/fonts.h"
#include <stdbool.h>

#define EPAPER_WIDTH 200
#define EPAPER_HEIGHT 200
#define EPAPER_IMAGE_SIZE ((EPAPER_WIDTH % 8 == 0) ? (EPAPER_WIDTH / 8) : (EPAPER_WIDTH / 8 + 1)) * EPAPER_HEIGHT

#define EPAPER_BLACK 0x00
#define EPAPER_WHITE 0xFF

bool epaper_is_busy(void);
void epaper_init(void);
void epaper_partial_init(void);
void epaper_clear(uint16_t color);
void epaper_partial_display(uint8_t *image, bool partial);
void epaper_partial_upload_base_image(uint8_t *image);
void epaper_sleep(void);

#endif
