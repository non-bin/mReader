#ifndef __FONTS_H
#define __FONTS_H

#define MAX_HEIGHT_FONT 41
#define MAX_WIDTH_FONT 32
#define OFFSET_BITMAP

#include "config.h"
#include <stdint.h>

#define CHAR(row1, ...)                                         \
  {                                                             \
    sizeof(#row1) - 3, (const uint32_t[]) { row1, __VA_ARGS__ } \
  }

typedef struct
{
  uint16_t width;
  const uint32_t *bitmap;
} font_character_t;

typedef const struct
{
  const font_character_t *characters;
  uint16_t Height;
  uint16_t CharSpacing;
  uint16_t LineSpacing;
} font_t;

extern font_t font3;
extern font_t font4;
extern font_t font5;
extern font_t font6;
extern font_t font8;
extern font_t font12;
extern font_t font16;
extern font_t font24;
extern font_t font20;
#define FONT_COUNT 9
extern font_t *const fonts[FONT_COUNT];

#endif /* __FONTS_H */
