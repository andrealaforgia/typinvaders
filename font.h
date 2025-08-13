#ifndef FONT_H_
#define FONT_H_

#include "graphics.h"
#include "sprite.h"

void load_font_from_sprite_sheet(const sprite_sheet_ptr sprite_sheet);

void write(const graphics_context_ptr graphics_context,
           char *text,
           int x,
           int y,
           int zoom);

#endif  // FONT_H_
