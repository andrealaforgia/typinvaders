#include "intro_stage.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "animate.h"
#include "asteroid.h"
#include "audio.h"
#include "clock.h"
#include "events.h"
#include "frame.h"
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

static int title_text_scale;
static text_dimensions_t title_text_dimensions;
static point_t title_text_position;

static int instructions_text_scale;
static text_dimensions_t instructions_text_dimensions;
static point_t instructions_text_0_position;
static point_t instructions_text_1_position;
static point_t instructions_text_2_position;
static point_t instructions_text_3_position;
static point_t instructions_text_4_position;

static int action_text_scale;
static text_dimensions_t action_text_dimensions;
static point_t action_text_position;

static text_dimensions_t copyright_text_dimensions;
static int copyright_text_scale;

static sprite_t font[256] = {0};

void init_intro_stage(const game_ptr _game) {
    game = _game;
    graphics_context = &game->graphics_context;

    title_text_scale = (graphics_context->screen_height * 25) / 900;
    title_text_dimensions =
        calculate_text_dimensions(TITLE_TEXT, title_text_scale);
    title_text_position = point(
        graphics_context->screen_center.x - title_text_dimensions.width / 2,
        graphics_context->screen_center.y -
            graphics_context->screen_height / 3);

    action_text_scale = (graphics_context->screen_height * 10) / 900;
    action_text_dimensions =
        calculate_text_dimensions(ACTION_TEXT, action_text_scale);
    action_text_position = point(
        graphics_context->screen_center.x - action_text_dimensions.width / 2,
        graphics_context->screen_center.y +
            graphics_context->screen_height / 6);

    instructions_text_scale = (graphics_context->screen_height * 10) / 900;
    instructions_text_dimensions =
        calculate_text_dimensions(INSTRUCTIONS_TEXT_0, instructions_text_scale);
    instructions_text_0_position =
        point(graphics_context->screen_center.x -
                  instructions_text_dimensions.width / 2,
              graphics_context->screen_center.y -
                  graphics_context->screen_height / 4);

    instructions_text_1_position =
        point(graphics_context->screen_center.x -
                  instructions_text_dimensions.width / 2,
              graphics_context->screen_center.y -
                  graphics_context->screen_height / 4 +
                  instructions_text_dimensions.height +
                  instructions_text_dimensions.height / 2);

    instructions_text_2_position =
        point(graphics_context->screen_center.x -
                  instructions_text_dimensions.width / 2,
              graphics_context->screen_center.y -
                  graphics_context->screen_height / 4 +
                  instructions_text_dimensions.height * 2 +
                  2 * instructions_text_dimensions.height / 2);

    instructions_text_3_position =
        point(graphics_context->screen_center.x -
                  instructions_text_dimensions.width / 2,
              graphics_context->screen_center.y -
                  graphics_context->screen_height / 4 +
                  instructions_text_dimensions.height * 3 +
                  3 * instructions_text_dimensions.height / 2);

    instructions_text_4_position =
        point(graphics_context->screen_center.x -
                  instructions_text_dimensions.width / 2,
              graphics_context->screen_center.y -
                  graphics_context->screen_height / 4 +
                  instructions_text_dimensions.height * 4 +
                  4 * instructions_text_dimensions.height / 2);

    copyright_text_scale = (graphics_context->screen_height * 5) / 900;
    copyright_text_dimensions =
        calculate_text_dimensions(COPYRIGHT_TEXT, copyright_text_scale);
}

void load_font_from_sprite_sheet(const sprite_sheet_ptr sprite_sheet) {
    const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!0123456789-";
    int x = 0;
    int y = 0;
    int i = 0;
    while (chars[i]) {
        if (chars[i] == 'P' || chars[i] == '0') {
            x = 0;
            y += 8;
        }
        font[(int)chars[i]] = create_sprite(sprite_sheet, x, y, 8, 8);
        x += 8;
        ++i;
    }
}

void write(const graphics_context_ptr graphics_context, char* text, int x,
           int y, int zoom) {
    int char_x = x;
    for (int i = 0; text[i]; i++) {
        if (text[i] == ' ') {
            char_x += 4 * zoom;
            continue;
        }
        render_sprite(graphics_context, &font[(int)(text[i])], char_x, y, 0,
                      zoom);
        char_x += 8 * zoom;
    }
}

typedef struct pacman_t {
    sprite_t right_sprites[4];
    sprite_t death_sprites[9];
    int sprite_index;
} pacman_t;

typedef struct maze_t {
    sprite_t parts[44];
    sprite_t layout[31][28];
} maze_t;

game_stage_action_t handle_intro_stage(void) {
    int last_action_text_ticks = get_clock_ticks_ms();

    bool is_action_text_on = true;

    asteroid_t asteroids[ASTEROIDS_COUNT];
    for (int i = 0; i < ASTEROIDS_COUNT; i++) {
        asteroids[i] = create_asteroid(random_point(graphics_context),
                                       random_asteroid_scale(), random_color());
    }

    int last_frame_ticks = get_clock_ticks_ms();

    sprite_sheet_t font_sprite_sheet =
        create_sprite_sheet(graphics_context, "./assets/sprites/font.png");
    load_font_from_sprite_sheet(&font_sprite_sheet);

    sprite_sheet_t sprite_sheet =
        create_sprite_sheet(graphics_context, "./assets/sprites/sprites.png");

    pacman_t pacman;
    pacman.sprite_index = 0;
    pacman.right_sprites[0] = create_sprite(&sprite_sheet, 456, 0, 16, 16);
    pacman.right_sprites[1] = create_sprite(&sprite_sheet, 456 + 16, 0, 16, 16);
    pacman.right_sprites[2] = create_sprite(&sprite_sheet, 456 + 32, 0, 16, 16);
    pacman.right_sprites[3] = pacman.right_sprites[1];
    int pacman_last_ticks = last_frame_ticks;

    maze_t maze;
    int maze_x = 28 * 9;
    int maze_y = 0;
    for (int i = 0; i < 28; i++) {
        maze.parts[i] = create_sprite(&sprite_sheet, maze_x, maze_y, 8, 8);
        maze_x += 8;
    }
  
    sprite_sheet_t total = create_sprite_sheet(graphics_context, "./assets/sprites/sprites.png");

    sprite_t total_sprite = create_sprite(&total, 0, 0, 680, 248);

    int death_sprite_ticks = get_clock_ticks_ms();
    int death_sprite_index = 0;
    
    while (true) {
        if (elapsed_from(last_frame_ticks) < (1000 / game->settings.fps)) {
            continue;
        }
        last_frame_ticks = get_clock_ticks_ms();

        clear_frame(graphics_context);

        //    write(graphics_context, "PRESS ANY KEY TO START!",
        //    graphics_context->screen_width/2,
        //    graphics_context->screen_height/2, 8);

        // for (int i = 0; i < 28; i++) {
        //     render_sprite(graphics_context, &maze.parts[i], i * 8 * 2 + 100,
        //                   100, 0, 2);
        // }
     
        int zoom = 1;
      
      //  render_sprite(graphics_context, &total_sprite, 0, 0, 0, zoom); 

        // if (last_frame_ticks - pacman_last_ticks > 50) {
        //     pacman.sprite_index++;
        //     if (pacman.sprite_index == 4) {
        //         pacman.sprite_index = 0;
        //     }
        //     pacman_last_ticks = get_clock_ticks_ms();
        // }

        // render_sprite(graphics_context,
        //               &pacman.right_sprites[pacman.sprite_index],
        //               graphics_context->screen_width / 2,
        //               graphics_context->screen_height / 4, 0, zoom);
                  
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
        //   draw_line(graphics_context, 0, i*8*zoom, 5000, i*8*zoom, COLOR(0,255, 255));
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

        sprite_t death_sprites[11];
         death_sprites[0] = create_sprite(&total, 456+16*3, 0, 16, 16);
          death_sprites[1] = create_sprite(&total, 456+16*4, 0, 16, 16);
          death_sprites[2] = create_sprite(&total, 456+16*5, 0, 16, 16);
          death_sprites[3] = create_sprite(&total, 456+16*6, 0, 16, 16);
          death_sprites[4] = create_sprite(&total, 456+16*7, 0, 16, 16);
          death_sprites[5] = create_sprite(&total, 456+16*8, 0, 16, 16);
          death_sprites[6] = create_sprite(&total, 456+16*9, 0, 16, 16);
          death_sprites[7] = create_sprite(&total, 456+16*10, 0, 16, 16);
          death_sprites[8] = create_sprite(&total, 456+16*11, 0, 16, 16);
          death_sprites[9] = create_sprite(&total, 456+16*12, 0, 16, 16);
          death_sprites[10] = create_sprite(&total, 456+16*13, 0, 16, 16);
        
        if (get_clock_ticks_ms() - death_sprite_ticks > 150) {
          death_sprite_index++;
          if (death_sprite_index == 11) {
            death_sprite_index = 0;
          }
          death_sprite_ticks = get_clock_ticks_ms();
        }

          render_sprite(graphics_context, &death_sprites[death_sprite_index], 500, 100, 0, 4);
        // for (int i = 0; i < ASTEROIDS_COUNT; i++) {
        //     wrap_animate(graphics_context, &asteroids[i].position,
        //                  &asteroids[i].velocity);
        //     render_asteroid(graphics_context, &asteroids[i]);
        // }

        // write_text(graphics_context, INSTRUCTIONS_TEXT_0,
        //            instructions_text_0_position, instructions_text_scale,
        //            COLOR_GRAY);

        // write_text(graphics_context, INSTRUCTIONS_TEXT_1,
        //            instructions_text_1_position, instructions_text_scale,
        //            COLOR_GRAY);

        // write_text(graphics_context, INSTRUCTIONS_TEXT_2,
        //            instructions_text_2_position, instructions_text_scale,
        //            COLOR_GRAY);

        // write_text(graphics_context, INSTRUCTIONS_TEXT_3,
        //            instructions_text_3_position, instructions_text_scale,
        //            COLOR_GRAY);

        // write_text(graphics_context, INSTRUCTIONS_TEXT_4,
        //            instructions_text_4_position, instructions_text_scale,
        //            COLOR_GRAY);

        // if (elapsed_from(last_action_text_ticks) > ACTION_TEXT_FLASHING_TICKS) {
        //     is_action_text_on = !is_action_text_on;
        //     last_action_text_ticks = get_clock_ticks_ms();
        // }

        // write_text(graphics_context, TITLE_TEXT, title_text_position,
        //            title_text_scale, COLOR_YELLOW);

        // if (is_action_text_on) {
        //     write_text(graphics_context, ACTION_TEXT, action_text_position,
        //                action_text_scale, COLOR_WHITE);
        // }

        // write_text(graphics_context, COPYRIGHT_TEXT,
        //            point(graphics_context->screen_center.x -
        //                      copyright_text_dimensions.width / 2,
        //                  graphics_context->screen_height -
        //                      copyright_text_dimensions.height - 5),
        //            copyright_text_scale, COLOR_DARK_YELLOW);

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

        if (is_space_key_pressed(&game->keyboard_state)) {
            return PROGRESS;
        }

        if (is_esc_key_pressed(&game->keyboard_state)) {
            return QUIT;
        }
    }
}
