#ifndef PACMAN_H_
#define PACMAN_H_

#include "direction.h"
#include "geometry.h"
#include "graphics.h"
#include "sprite.h"

typedef enum { PM_ALIVE, PM_DEAD } pm_status_t;

typedef struct pacman_t {
    float x;
    float y;
    direction_t dir;
    pm_status_t status;
    int mouth_phase;
    int anim_acc;
    float speed_pps;
    sprite_t frames_closed;
    sprite_t frames_dir[4][2];
} pacman_t;

typedef pacman_t *pacman_ptr;

pacman_t pacman_create(sprite_sheet_ptr sheet, point_t start_pos);
void pacman_destroy(pacman_ptr p);
void pacman_set_direction(pacman_ptr p, direction_t dir);
direction_t pacman_direction(const pacman_ptr p);
void pacman_update(pacman_ptr p, int dt_ms);
sprite_t pacman_current_sprite(const pacman_ptr p);
point_t pacman_position(const pacman_ptr p);
void pacman_render(const pacman_ptr p, const graphics_context_ptr gc, int zoom);

#endif  // PACMAN_H_
