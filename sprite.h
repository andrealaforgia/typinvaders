#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"

typedef struct {
  SDL_Texture* texture;
} sprite_sheet_t, *sprite_sheet_ptr;

typedef struct {
  sprite_sheet_ptr sheet;
  int x;
  int y;
  int size;
} sprite_t, *sprite_ptr;

sprite_sheet_t create_sprite_sheet(const graphics_context_ptr graphics_context,
                                   const char* texture_path);
sprite_t create_sprite(const sprite_sheet_ptr sprite_sheet, int x, int y,
                       int size);
void render_sprite(const graphics_context_ptr graphics_context,
                   const sprite_ptr sprite, int screen_x, int screen_y,
                   float angle);
void free_sprite_sheet(const sprite_sheet_ptr sprite_sheet);

#endif  // SPRITE_H_
