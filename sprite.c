#include "sprite.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"

sprite_sheet_t create_sprite_sheet(const graphics_context_ptr graphics_context,
                                   const char* texture_path) {
  sprite_sheet_t sprite_sheet;
  sprite_sheet.texture =
      IMG_LoadTexture(graphics_context->renderer, texture_path);
  if (!sprite_sheet.texture) {
    fprintf(stderr, "Failed to load sprite sheet texture: %s\n",
            IMG_GetError());
    free_sprite_sheet(&sprite_sheet);
    exit(EXIT_FAILURE);
  }
  return sprite_sheet;
}

void free_sprite_sheet(const sprite_sheet_ptr sprite_sheet) {
  if (!sprite_sheet) return;
  if (sprite_sheet->texture) {
    SDL_DestroyTexture(sprite_sheet->texture);
  }
  free(sprite_sheet);
}

sprite_t create_sprite(const sprite_sheet_ptr sprite_sheet, int x, int y,
                       int size) {
  sprite_t sprite;
  sprite.sheet = sprite_sheet;
  sprite.x = x;
  sprite.y = y;
  sprite.size = size;
  return sprite;
}

void render_sprite(const graphics_context_ptr graphics_context,
                   const sprite_ptr sprite, int screen_x, int screen_y,
                   float angle) {
  SDL_Rect src_rect = {sprite->x, sprite->y, sprite->size, sprite->size};
  SDL_Rect dst_rect = {screen_x, screen_y, sprite->size, sprite->size};
  SDL_Point center = {sprite->size / 2, sprite->size / 2};
  SDL_RenderCopyEx(graphics_context->renderer, sprite->sheet->texture,
                   &src_rect, &dst_rect, angle, &center, SDL_FLIP_NONE);
}
