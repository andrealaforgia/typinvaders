#include "intro_stage.h"

#include "clock.h"
#include "direction.h"
#include "events.h"
#include "font.h"
#include "frame.h"
#include "game.h"
#include "geometry.h"
#include "graphics.h"
#include "keyboard.h"
#include "maze.h"
#include "pacman_character.h"
#include "power_pellet.h"
#include "render.h"
#include "sprite.h"

static game_ptr game = NULL;
static graphics_context_ptr graphics_context = NULL;

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

    maze_t maze = create_maze(&sprite_sheet, DEFAULT_MAZE_SYMBOLS, 31);

    int zoom = calculate_maze_zoom(graphics_context, &maze);

    rectangle_t maze_rectangle =
        create_maze_rectangle(graphics_context, zoom, &maze);

    pacman_character_t pacman = pacman_character_create(
        &sprite_sheet,
        point(maze_rectangle.top_left.x + 13 * 8 * zoom + 1 * zoom,
              maze_rectangle.top_left.y + 23 * 8 * zoom - 4 * zoom));

    power_pellet_t power_pellet = power_pellet_create();

    while (1) {
        int now = get_clock_ticks_ms();
        int dt = now - last_frame_ticks;

        int delay = (1000 / game->settings.fps) - dt;
        if (delay > 0)
            SDL_Delay(delay);

        last_frame_ticks = get_clock_ticks_ms();

        power_pellet_update(&power_pellet, now);

        clear_frame(graphics_context);

        pacman_character_update(&pacman, dt);
        render_maze(graphics_context,
                    &maze,
                    maze_rectangle,
                    zoom,
                    power_pellet_get_status(&power_pellet) == PPS_VISIBLE);
        pacman_character_render(&pacman, graphics_context, zoom);

        render_frame(graphics_context);

        event_t event;
        while ((event = poll_event())) {
            if (event == QUIT_EVENT)
                return QUIT;
        }

        if (is_up_key_pressed(&game->keyboard_state))
            pacman_character_set_direction(&pacman, DIR_UP);
        else if (is_down_key_pressed(&game->keyboard_state))
            pacman_character_set_direction(&pacman, DIR_DOWN);
        else if (is_left_key_pressed(&game->keyboard_state))
            pacman_character_set_direction(&pacman, DIR_LEFT);
        else if (is_right_key_pressed(&game->keyboard_state))
            pacman_character_set_direction(&pacman, DIR_RIGHT);

        if (is_esc_key_pressed(&game->keyboard_state))
            return QUIT;
    }

    pacman_character_destroy(&pacman);
}
