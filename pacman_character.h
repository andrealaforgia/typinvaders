#ifndef PACMAN_CHARACTER_H_
#define PACMAN_CHARACTER_H_

#include "direction.h"
#include "geometry.h"
#include "graphics.h"
#include "sprite.h"

typedef enum { PM_ALIVE, PM_DEAD } pm_status_t;

typedef struct pacman_character_t {
    float x;
    float y;
    direction_t dir;
    pm_status_t status;
    int mouth_phase;
    int anim_acc;
    float speed_pps;
    sprite_t frames_closed;
    sprite_t frames_dir[4][2];
} pacman_character_t;

typedef pacman_character_t *pacman_character_ptr;

pacman_character_t pacman_character_create(sprite_sheet_ptr sheet,
                                           point_t start_pos);
void pacman_character_destroy(pacman_character_ptr p);
void pacman_character_set_direction(pacman_character_ptr p, direction_t dir);
direction_t pacman_character_direction(const pacman_character_ptr p);
void pacman_character_update(pacman_character_ptr p, int dt_ms);
sprite_t pacman_character_current_sprite(const pacman_character_ptr p);
point_t pacman_character_position(const pacman_character_ptr p);
void pacman_character_render(const pacman_character_ptr p,
                             const graphics_context_ptr gc,
                             int zoom);

#endif  // PACMAN_CHARACTER_H_
