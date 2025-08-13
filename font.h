#ifndef FONT_H
#define FONT_H

#include "graphics.h"
#include "sprite.h"

/**
 * Loads font sprites from a sprite sheet
 * @param sprite_sheet Pointer to the sprite sheet containing font characters
 */
void load_font_from_sprite_sheet(const sprite_sheet_ptr sprite_sheet);

/**
 * Renders text on screen using the loaded font
 * @param graphics_context Pointer to the graphics context
 * @param text The text string to render
 * @param x X coordinate for text position
 * @param y Y coordinate for text position
 * @param zoom Zoom factor for text scaling
 */
void write(const graphics_context_ptr graphics_context,
           char *text,
           int x,
           int y,
           int zoom);

#endif  // FONT_H
