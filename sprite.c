#include "sprite.h"

#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"
#include "graphics.h"

sprite_t create_sprite(const graphics_context_ptr graphics_context,
                       const char* texture_path) {
  sprite_t sprite;
  sprite.texture = IMG_LoadTexture(graphics_context->renderer, texture_path);
  sprite.sprite_index = 0;
  sprite.angle = 0.0;
  if (!sprite.texture) {
    fprintf(stderr, "Failed to load sprite texture: %s\n", IMG_GetError());
    cleanup(&sprite);
    exit(EXIT_FAILURE);
  }
  return sprite;
}

void render_sprite(const graphics_context_ptr graphics_context,
                   const sprite_ptr sprite, int x, int y, int sprite_size) {
  SDL_Rect src_rect = {sprite->sprite_index * sprite_size, 0, sprite_size,
                       sprite_size};
  SDL_Rect dst_rect = {x, y, sprite_size, sprite_size};
  SDL_Point center = {sprite_size / 2, sprite_size / 2};
  SDL_RenderCopyEx(graphics_context->renderer, sprite->texture, &src_rect,
                   &dst_rect, sprite->angle, &center, SDL_FLIP_NONE);
}

void cleanup(const sprite_ptr sprite) { SDL_DestroyTexture(sprite->texture); }
