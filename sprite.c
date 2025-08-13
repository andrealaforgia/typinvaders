#include "sprite.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "inline.h"

sprite_sheet_t create_sprite_sheet(const graphics_context_ptr graphics_context,
                                   const char *texture_path) {
    sprite_sheet_t sprite_sheet;

    // Initialize SDL_image if not already done
    static int img_initialized = 0;
    if (!img_initialized) {
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            fprintf(stderr,
                    "SDL_image initialization failed: %s\n",
                    IMG_GetError());
            exit(EXIT_FAILURE);
        }
        img_initialized = 1;
    }

    SDL_Surface *surface = IMG_Load(texture_path);
    if (!surface) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // Set color key to make black (0,0,0) transparent
    Uint32 color_key = SDL_MapRGB(surface->format, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, color_key);

    sprite_sheet.texture =
        SDL_CreateTextureFromSurface(graphics_context->renderer, surface);
    SDL_FreeSurface(surface);

    if (!sprite_sheet.texture) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return sprite_sheet;
}

void free_sprite_sheet(const sprite_sheet_ptr sprite_sheet) {
    if (!sprite_sheet)
        return;
    if (sprite_sheet->texture) {
        SDL_DestroyTexture(sprite_sheet->texture);
    }
}

sprite_t create_sprite(
    const sprite_sheet_ptr sprite_sheet, int x, int y, int width, int height) {
    sprite_t sprite;
    sprite.sheet = sprite_sheet;
    sprite.x = x;
    sprite.y = y;
    sprite.width = width;
    sprite.height = height;
    return sprite;
}

ALWAYS_INLINE void render_sprite(const graphics_context_ptr graphics_context,
                                 const sprite_ptr sprite,
                                 int screen_x,
                                 int screen_y,
                                 float angle,
                                 float zoom) {
    SDL_Rect src_rect = { sprite->x, sprite->y, sprite->width, sprite->height };
    SDL_Rect dst_rect = { screen_x,
                          screen_y,
                          (int)(sprite->width * zoom),
                          (int)(sprite->height * zoom) };

    SDL_Point center = { dst_rect.w / 2, dst_rect.h / 2 };

    SDL_RenderCopyEx(graphics_context->renderer,
                     sprite->sheet->texture,
                     &src_rect,
                     &dst_rect,
                     angle,
                     &center,
                     SDL_FLIP_NONE);
}
