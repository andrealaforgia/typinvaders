#ifndef SPRITE__H
#define SPRITE__H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "geometry.h"

typedef struct {
    SDL_Texture* texture;
    int sprite_index;
    double angle;
} sprite_t, *sprite_ptr;

sprite_t create_sprite(const graphics_context_ptr context, const char* texture_path); 
void render_sprite(const graphics_context_ptr context, const sprite_ptr sprite, int x, int y, int sprite_size);
void cleanup(const sprite_ptr sprite);

#endif

