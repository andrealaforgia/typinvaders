#include "intro_stage.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "animate.h"
#include "asteroid.h"
#include "audio.h"
#include "clock.h"
#include "events.h"
#include "font.h"
#include "frame.h"
#include "maze.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "hash_map.h"
#include "keyboard.h"
#include "physics.h"
#include "render.h"
#include "saucer.h"
#include "sprite.h"
#include "text.h"

#define TITLE_TEXT "ASTEROIDS"
#define ACTION_TEXT "PRESS SPACE TO START"
#define ACTION_TEXT_FLASHING_TICKS 750

#define INSTRUCTIONS_TEXT_0 "        PRESS UP OR K TO THRUST SHIP        "
#define INSTRUCTIONS_TEXT_1 "PRESS LEFT OR H OR RIGHT OR L TO ROTATE SHIP"
#define INSTRUCTIONS_TEXT_2 "            PRESS SPACE TO SHOOT            "
#define INSTRUCTIONS_TEXT_3 "     PRESS S TO ENABLE OR DISABLE SOUND     "
#define INSTRUCTIONS_TEXT_4 "              PRESS ESC TO EXIT             "

#define ASTEROIDS_COUNT 150



static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;

void init_intro_stage(const game_ptr _game) {
    game = _game;
    graphics_context = &game->graphics_context;
}





typedef struct rendered_sprite_t {
    sprite_t sprite;
    point_t position;
} rendered_sprite_t;

typedef enum pacman_status_t { ALIVE, DEAD } pacman_status_t;

typedef enum direction_t { UP, DOWN, LEFT, RIGHT } direction_t;

typedef enum pacman_stage_t {
    CLOSED = 0,
    OPENING = 1,
    OPEN = 2,
    CLOSING = 3
} pacman_stage_t;

#define DEATH_STAGE_COUNT 11

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

game_stage_action_t handle_intro_stage(void) {
    int last_action_text_ticks = get_clock_ticks_ms();

    bool is_action_text_on = true;

    asteroid_t asteroids[ASTEROIDS_COUNT];
    for (int i = 0; i < ASTEROIDS_COUNT; i++) {
        asteroids[i] = create_asteroid(random_point(graphics_context),
                                       random_asteroid_scale(),
                                       random_color());
    }

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

    sprite_sheet_t total =
        create_sprite_sheet(graphics_context, "./assets/sprites/sprites.png");

    sprite_t total_sprite = create_sprite(&total, 0, 0, 680, 248);

    int death_sprite_ticks = get_clock_ticks_ms();
    int death_sprite_index = 0;



    sprite_t pacman_sprite = get_pacman_sprite(pacman.stage, pacman.direction);

    while (true) {
        int delay =
            (1000 / game->settings.fps) - elapsed_from(last_frame_ticks);
        if (delay > 0) {
            SDL_Delay(delay);
        }
        last_frame_ticks = get_clock_ticks_ms();

        clear_frame(graphics_context);

        // render_sprite(graphics_context, &total_sprite, 0, 0, 0, zoom);

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

        // int y = 0;
        // int x = 0;
        // for (int i=0; i<29; i++){
        //   draw_line(graphics_context, x, 0, x, 3000, COLOR_YELLOW);
        //   // if (i == 28){
        //   //   x -= 10;
        //   // } else if (i == 57) {
        //   //   x -= 14;
        //   // }
        //   x += 8*zoom;
        // }

        // x = 28*8*zoom + 4*zoom;
        // printf("maze x: %d\n", x);
        // for (int i=0; i<29; i++){
        //   draw_line(graphics_context, x, 0, x, 3000, COLOR(0, 255, 0));
        //   // if (i == 28){
        //   //   x -= 10;
        //   // } else if (i == 57) {
        //   //   x -= 14;
        //   // }
        //   x += 8*zoom;
        // }

        // for (int i=0; i<100; i++){
        //   draw_line(graphics_context, 0, i*8*zoom, 5000, i*8*zoom,
        //   COLOR(0,255, 255));
        // }

        // x = 56*8*zoom + 8*zoom;
        // printf("sprites x: %d\n", x);
        // for (int i=0; i<50; i++){
        //   draw_line(graphics_context, x, 0, x, 3000, COLOR_WHITE);
        //   // if (i == 28){
        //   //   x -= 10;
        //   // } else if (i == 57) {
        //   //   x -= 14;
        //   // }
        //   x += 8*zoom;
        // }

        // sprite_t death_sprites[11];
        //  death_sprites[0] = create_sprite(&total, 456+16*3, 0, 16, 16);
        //   death_sprites[1] = create_sprite(&total, 456+16*4, 0, 16, 16);
        //   death_sprites[2] = create_sprite(&total, 456+16*5, 0, 16, 16);
        //   death_sprites[3] = create_sprite(&total, 456+16*6, 0, 16, 16);
        //   death_sprites[4] = create_sprite(&total, 456+16*7, 0, 16, 16);
        //   death_sprites[5] = create_sprite(&total, 456+16*8, 0, 16, 16);
        //   death_sprites[6] = create_sprite(&total, 456+16*9, 0, 16, 16);
        //   death_sprites[7] = create_sprite(&total, 456+16*10, 0, 16, 16);
        //   death_sprites[8] = create_sprite(&total, 456+16*11, 0, 16, 16);
        //   death_sprites[9] = create_sprite(&total, 456+16*12, 0, 16, 16);
        //   death_sprites[10] = create_sprite(&total, 456+16*13, 0, 16, 16);

        // if (get_clock_ticks_ms() - death_sprite_ticks > 150) {
        //   death_sprite_index++;
        //   if (death_sprite_index == 11) {
        //     death_sprite_index = 0;
        //   }
        //   death_sprite_ticks = get_clock_ticks_ms();
        // }

        // render_sprite(graphics_context, &death_sprites[death_sprite_index],
        // 500, 100, 0, 4);

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
