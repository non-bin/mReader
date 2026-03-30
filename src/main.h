#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

typedef enum
{
  BUTTON_NEXT,
  BUTTON_PREVIOUS,
  BUTTON_ENTER,
  BUTTON_BACK
} button_action_t;

typedef enum
{
  PAGE_CATALOG,
  PAGE_READER,
  PAGE_FONT_SIZE
} page_t;

typedef struct
{
  uint64_t scroll;
  page_t current_page;
  page_t history[HISTORY_LENGTH];
  uint16_t history_index;
  uint16_t font_index;
  uint16_t fg_color;
  uint16_t bg_color;
} state_t;

#endif /* __MAIN_H */
