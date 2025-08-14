#include "pacman.h"

#define PACMAN_SPEED_PPS 80
#define PACMAN_ANIM_FRAME_MS 67
#define PACMAN_MOUTH_STATES 4

static int dir_index(direction_t d) {
    switch (d) {
    case DIR_RIGHT:
        return 0;
    case DIR_LEFT:
        return 1;
    case DIR_UP:
        return 2;
    case DIR_DOWN:
        return 3;
    }
    return 0;
}

static sprite_t select_frame(const pacman_ptr p) {
    if (p->mouth_phase == 0)
        return p->frames_closed;
    int di = dir_index(p->dir);
    int variant = (p->mouth_phase & 1);
    return p->frames_dir[di][variant];
}

pacman_t pacman_create(sprite_sheet_ptr sheet, point_t start_pos) {
    pacman_t p;
    p.x = (float)start_pos.x;
    p.y = (float)start_pos.y;
    p.dir = DIR_LEFT;
    p.status = PM_ALIVE;
    p.mouth_phase = 0;
    p.anim_acc = 0;
    p.speed_pps = PACMAN_SPEED_PPS;
    int base_x = 456;
    p.frames_dir[0][0] = create_sprite(sheet, base_x + 16, 0, 16, 16);
    p.frames_dir[0][1] = create_sprite(sheet, base_x, 0, 16, 16);
    p.frames_dir[1][0] = create_sprite(sheet, base_x + 16, 16, 16, 16);
    p.frames_dir[1][1] = create_sprite(sheet, base_x, 16, 16, 16);
    p.frames_dir[2][0] = create_sprite(sheet, base_x + 16, 32, 16, 16);
    p.frames_dir[2][1] = create_sprite(sheet, base_x, 32, 16, 16);
    p.frames_dir[3][0] = create_sprite(sheet, base_x + 16, 48, 16, 16);
    p.frames_dir[3][1] = create_sprite(sheet, base_x, 48, 16, 16);
    p.frames_closed = create_sprite(sheet, base_x + 32, 0, 16, 16);
    return p;
}

void pacman_destroy(pacman_ptr p) { (void)p; }

void pacman_set_direction(pacman_ptr p, direction_t dir) {
    if (!p)
        return;
    p->dir = dir;
}

direction_t pacman_direction(const pacman_ptr p) {
    return p ? p->dir : DIR_RIGHT;
}

void pacman_update(pacman_ptr p, int dt_ms) {
    if (!p || p->status != PM_ALIVE)
        return;
    float dist = p->speed_pps * (dt_ms / 1000.0f);
    p->x += dist * direction_dx(p->dir);
    p->y += dist * direction_dy(p->dir);
    p->anim_acc += dt_ms;
    while (p->anim_acc >= PACMAN_ANIM_FRAME_MS) {
        p->mouth_phase = (p->mouth_phase + 1) % PACMAN_MOUTH_STATES;
        p->anim_acc -= PACMAN_ANIM_FRAME_MS;
    }
}

sprite_t pacman_current_sprite(const pacman_ptr p) { return select_frame(p); }

point_t pacman_position(const pacman_ptr p) {
    point_t pt = { (int)p->x, (int)p->y };
    return pt;
}

void pacman_render(const pacman_ptr p,
                   const graphics_context_ptr gc,
                   int zoom) {
    if (!p)
        return;
    sprite_t s = select_frame(p);
    point_t pos = pacman_position(p);
    render_sprite(gc, &s, pos.x, pos.y, 0, zoom);
}
