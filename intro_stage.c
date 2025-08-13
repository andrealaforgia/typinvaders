#include "intro_stage.h"

#include <stdbool.h>

#include "clock.h"
#include "events.h"
#include "font.h"
#include "frame.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "keyboard.h"
#include "maze.h"
#include "render.h"
#include "sprite.h"

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;

typedef enum pacman_status_t { ALIVE, DEAD } pacman_status_t;

typedef enum direction_t { UP, DOWN, LEFT, RIGHT } direction_t;

typedef enum pacman_stage_t {
    CLOSED = 0,
    OPENING = 1,
    OPEN = 2,
    CLOSING = 3
} pacman_stage_t;

typedef struct pacman_t {
    point_t position;
    direction_t direction;
    pacman_status_t status;
    pacman_stage_t stage;
} pacman_t;

sprite_t pacman_sprites[9];

sprite_t get_pacman_sprite(pacman_stage_t stage, direction_t dir) {
    if (stage == CLOSED) {
        return pacman_sprites[0];
    }

    int base_index = 0;

    switch (dir) {
    case RIGHT:
        base_index = 1;
        break;
    case LEFT:
        base_index = 3;
        break;
    case UP:
        base_index = 5;
        break;
    case DOWN:
        base_index = 7;
        break;
    }

    if (stage == OPENING || stage == CLOSING) {
        return pacman_sprites[base_index];  // partially open
    } else if (stage == OPEN) {
        return pacman_sprites[base_index + 1];  // fully open
    }

    return pacman_sprites[0];  // fallback
}

pacman_stage_t next_stage(pacman_stage_t current_stage) {
    switch (current_stage) {
    case CLOSED:
        return OPENING;
    case OPENING:
        return OPEN;
    case OPEN:
        return CLOSING;
    case CLOSING:
        return CLOSED;
    default:
        return CLOSED;  // fallback in case of invalid input
    }
}

void init_intro_stage(const game_ptr _game) {
    game = _game;
    graphics_context = &game->graphics_context;
}

game_stage_action_t handle_intro_stage(void) {
    int last_frame_ticks = get_clock_ticks_ms();

    sprite_sheet_t font_sprite_sheet =
        create_sprite_sheet(graphics_context, "./assets/sprites/font.png");
    load_font_from_sprite_sheet(&font_sprite_sheet);

    sprite_sheet_t sprite_sheet =
        create_sprite_sheet(graphics_context, "./assets/sprites/sprites.png");

    pacman_sprites[0] =
        create_sprite(&sprite_sheet, 456 + 32, 0, 16, 16);  // Closed mouth

    pacman_sprites[1] = create_sprite(&sprite_sheet,
                                      456 + 16,
                                      0,
                                      16,
                                      16);  // Partially open mouth (right)
    pacman_sprites[2] = create_sprite(&sprite_sheet,
                                      456,
                                      0,
                                      16,
                                      16);  // Fully open mouth (right)

    pacman_sprites[3] = create_sprite(&sprite_sheet,
                                      456 + 16,
                                      16,
                                      16,
                                      16);  // Partially open mouth (left)
    pacman_sprites[4] = create_sprite(&sprite_sheet,
                                      456,
                                      16,
                                      16,
                                      16);  // Fully open mouth (left)

    pacman_sprites[5] = create_sprite(&sprite_sheet,
                                      456 + 16,
                                      32,
                                      16,
                                      16);  // Partially open mouth (up)
    pacman_sprites[6] =
        create_sprite(&sprite_sheet, 456, 32, 16, 16);  // Fully open mouth (up)

    pacman_sprites[7] = create_sprite(&sprite_sheet,
                                      456 + 16,
                                      48,
                                      16,
                                      16);  // Partially open mouth (down)
    pacman_sprites[8] = create_sprite(&sprite_sheet,
                                      456,
                                      48,
                                      16,
                                      16);  // Fully open mouth (down)

    pacman_t pacman;
    pacman.direction = LEFT;
    pacman.stage = CLOSED;
    pacman.position = point(graphics_context->screen_width / 2,
                            graphics_context->screen_height / 2);
    pacman.status = ALIVE;

    int pacman_last_ticks = last_frame_ticks;
    int pacman_stage_last_ticks = last_frame_ticks;

    int zoom = calculate_maze_zoom(graphics_context);
    rectangle_t maze_rectangle = create_maze_rectangle(graphics_context, zoom);
    maze_t maze = create_maze(&sprite_sheet);

    sprite_t pacman_sprite = get_pacman_sprite(pacman.stage, pacman.direction);

    while (true) {
        int delay =
            (1000 / game->settings.fps) - elapsed_from(last_frame_ticks);
        if (delay > 0) {
            SDL_Delay(delay);
        }
        last_frame_ticks = get_clock_ticks_ms();

        clear_frame(graphics_context);

        if (elapsed_from(pacman_last_ticks) > 500) {
            pacman_sprite = get_pacman_sprite(pacman.stage, pacman.direction);
            switch (pacman.direction) {
            case UP:
                pacman.position.y -= 1;
                break;
            case DOWN:
                pacman.position.y += 1;
                break;
            case LEFT:
                pacman.position.x -= 1;
                break;
            case RIGHT:
                pacman.position.x += 1;
                break;
            }
            pacman_last_ticks = get_clock_ticks_ms();
        }

        if (elapsed_from(pacman_stage_last_ticks) > 67) {
            pacman.stage = next_stage(pacman.stage);
            pacman_stage_last_ticks = get_clock_ticks_ms();
        }

        render_maze(graphics_context, &maze, maze_rectangle, zoom);

        render_sprite(graphics_context,
                      &pacman_sprite,
                      pacman.position.x,
                      pacman.position.y,
                      0,
                      zoom);

        render_frame(graphics_context);

        event_t event;

        while ((event = poll_event())) {
            switch (event) {
            case NO_EVENT:
            case KEY_PRESSED_EVENT:
            case OTHER_EVENT:
                break;
            case QUIT_EVENT: {
                return QUIT;
            }
            }
        }

        if (is_up_key_pressed(&game->keyboard_state)) {
            pacman.direction = UP;
        }

        if (is_down_key_pressed(&game->keyboard_state)) {
            pacman.direction = DOWN;
        }

        if (is_left_key_pressed(&game->keyboard_state)) {
            pacman.direction = LEFT;
        }

        if (is_right_key_pressed(&game->keyboard_state)) {
            pacman.direction = RIGHT;
        }

        if (is_esc_key_pressed(&game->keyboard_state)) {
            return QUIT;
        }
    }
}
