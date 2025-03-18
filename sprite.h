#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL.h>
#include <SDL_image.h>

#include "geometry.h"
#include "graphics.h"

typedef struct {
  SDL_Texture* texture;
  int sprite_index;
  double angle;
} sprite_t, *sprite_ptr;

sprite_t create_sprite(const graphics_context_ptr context,
                       const char* texture_path);
void render_sprite(const graphics_context_ptr context, const sprite_ptr sprite,
                   int x, int y, int sprite_size);
void cleanup(const sprite_ptr sprite);

#endif  // SPRITE_H_
