#ifndef RENDER_H_
#define RENDER_H_

#include "asteroid.h"
#include "bullet.h"
#include "coords.h"
#include "graphics.h"
#include "inline.h"
#include "saucer.h"
#include "ship.h"
  
  
 typedef struct {
  graphics_context_t graphics_context;
  sprite_sheet_t sprite_sheet;
 } render_context_t, *render_context_ptr; 

render_context_t create_render_context(const graphics_context_t graphics_context, const render_context_t render_context);
void render_object(const graphics_context_ptr graphics_context, bounds_t bounds,
                   const point_ptr position, int scale, int color);
void render_asteroid(const graphics_context_ptr graphics_context,
                     const asteroid_ptr asteroid);
void render_saucer(const graphics_context_ptr graphics_context,
                   const saucer_ptr saucer);
void render_ship(const render_context_ptr render_context,
                 const ship_ptr ship);
void render_lives(const graphics_context_ptr graphics_context, point_t position,
                  const int lives);
void render_bullet(const graphics_context_ptr graphics_context,
                   const bullet_ptr bullet, color_t color);

#endif  // RENDER_H_
