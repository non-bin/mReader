#ifndef _EPAPER_H_
#define _EPAPER_H_

#include "../Fonts/fonts.h"

uint8_t epaper_init(void);
void epaper_clear(void);
uint8_t *epaper_create_image_buffer(void);
void epaper_select_image_buffer(uint8_t *imageBuffer);
void epaper_draw_bitmap(uint8_t *imageBuffer, const uint8_t *bitmap);
void epaper_fill(uint16_t Color);
void epaper_display(uint8_t *imageBuffer);
void epaper_draw_number(uint16_t Xpoint, uint16_t Ypoint, int32_t Nummber, sFONT *Font, uint16_t Color_Foreground, uint16_t Color_Background);
void epaper_draw_string(uint16_t Xstart, uint16_t Ystart, const char *pString, sFONT *Font, uint16_t Color_Foreground, uint16_t Color_Background);

#endif
