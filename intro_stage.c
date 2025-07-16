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

// const char* maze_symbols[MAZE_HEIGHT] = {
// "0111111111111231111111111114",
// "5............67............8",
// "5.9AAB.9AAAB.67.9AAAB.9AAB.8",
// "5*6  7.6   7.67.6   7.6  7*8",
// "5.CDDE.CDDDE.CE.CDDDE.CDDE.8",
// "5..........................8",
// "5.9AAB.9B.9AAAAAAB.9B.9AAB.8",
// "5.CDDE.67.CDDB9DDE.67.CDDE.8",
// "5......67....67....67......8",
// "FGGGGH.6CDDB 67 9AAE7.IGGGGJ",
// "     5.69AAE CE CDDB7.8     ",
// "     5.67          67.8     ", 
// };

const char* maze_symbols[] = {
"0111111111111231111111111114",
"5............67............8",
"5.9AAB.9AAAB.67.9AAAB.9AAB.8",
"5*6  7.6   7.67.6   7.6  7*8",
"5.CDDE.CDDDE.CE.CDDDE.CDDE.8",
"5..........................8",
"5.9AAB.9B.9AAAAAAB.9B.9AAB.8",
"5.CDDE.67.CDDB9DDE.67.CDDE.8",
"5......67....67....67......8",
"FGGGGH.6C    67 9AAE7.IGGGGJ",
"     5.6C    CE CDDB7.8     ",
"     5.67          67.8     ",
"     5.67 KPPLLPPM 67.8     ",
"11111N.CE S      T CE.O11111",
"      .   S      T   .      ",
"GGGGGH.9B S      T 9B.IGGGGG",
"     5.67 QUUUUUUR 67.8     ",
"     5.67          67.8     ",
"     5.67 9AAAAAAB 67.8     ",
"01111N.CE CDDB9DDE CE.O11114",
};


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
 
    int maze_height = sizeof(maze_symbols) / sizeof(char*);
    int maze_width = strlen(maze_symbols[0]);
    
    maze_t maze;
    int maze_x = 28 * 9;
    int maze_y = 0;
    for (int i = 0; i < maze_width; i++) {
        maze.parts[i] = create_sprite(&sprite_sheet, maze_x, maze_y, 8, 8);
        maze_x += 8;
    }
  
    sprite_sheet_t total = create_sprite_sheet(graphics_context, "./assets/sprites/sprites.png");

    sprite_t total_sprite = create_sprite(&total, 0, 0, 680, 248);

    int death_sprite_ticks = get_clock_ticks_ms();
    int death_sprite_index = 0;

    sprite_t maze_parts[maze_height][maze_width];

    for (int x=0; x<maze_width; x++) { 
        for (int y=0; y<maze_height; y++) {
            maze_parts[y][x] = create_sprite(&total, x*8, y*8, 8, 8);
        }
    }
    
    while (true) {
        if (elapsed_from(last_frame_ticks) < (1000 / game->settings.fps)) {
            continue;
        }
        last_frame_ticks = get_clock_ticks_ms();

        clear_frame(graphics_context);
        
        int zoom = 4;
      
       render_sprite(graphics_context, &total_sprite, 0, 0, 0, zoom); 

        if (last_frame_ticks - pacman_last_ticks > 50) {
            pacman.sprite_index++;
            if (pacman.sprite_index == 4) {
                pacman.sprite_index = 0;
            }
            pacman_last_ticks = get_clock_ticks_ms();
        }

        render_sprite(graphics_context,
                      &pacman.right_sprites[pacman.sprite_index],
                      graphics_context->screen_width / 2,
                      graphics_context->screen_height / 4, 0, zoom);
                  
        int y = 0;
        int x = 0;
        for (int i=0; i<29; i++){
          draw_line(graphics_context, x, 0, x, 3000, COLOR_YELLOW);
          // if (i == 28){
          //   x -= 10;
          // } else if (i == 57) {
          //   x -= 14;
          // }
          x += 8*zoom;
        }
        
        x = 28*8*zoom + 4*zoom;
        printf("maze x: %d\n", x);
        for (int i=0; i<29; i++){
          draw_line(graphics_context, x, 0, x, 3000, COLOR(0, 255, 0));
          // if (i == 28){
          //   x -= 10;
          // } else if (i == 57) {
          //   x -= 14;
          // }
          x += 8*zoom;
        }

        for (int i=0; i<100; i++){
          draw_line(graphics_context, 0, i*8*zoom, 5000, i*8*zoom, COLOR(0,255, 255));
        }
        
        x = 56*8*zoom + 8*zoom;
        printf("sprites x: %d\n", x);
        for (int i=0; i<50; i++){
          draw_line(graphics_context, x, 0, x, 3000, COLOR_WHITE);
          // if (i == 28){
          //   x -= 10;
          // } else if (i == 57) {
          //   x -= 14;
          // }
          x += 8*zoom;
        }

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
          

        clear_frame(graphics_context);

        for (int x=0; x<maze_width; x++) { 
            for (int y=0; y<maze_height; y++) {
                sprite_t sprite;
                switch (maze_symbols[y][x]) {
                    case ' ': continue;
                    case '.': sprite = maze_parts[1][1]; break;
                    case '0': sprite = maze_parts[0][0]; break;
                    case '1': sprite = maze_parts[0][1]; break;
                    case '2': sprite = maze_parts[0][13]; break;
                    case '3': sprite = maze_parts[0][14]; break;
                    case '4': sprite = maze_parts[0][27]; break;
                    case '5': sprite = maze_parts[1][0]; break;
                    case '6': sprite = maze_parts[1][13]; break;
                    case '7': sprite = maze_parts[1][14]; break;
                    case '8': sprite = maze_parts[1][27]; break;
                    case '9': sprite = maze_parts[2][2]; break;
                    case 'A': sprite = maze_parts[2][3]; break;
                    case 'B': sprite = maze_parts[2][5]; break;
                    case 'C': sprite = maze_parts[4][2]; break;
                    case 'D': sprite = maze_parts[4][3]; break;
                    case 'E': sprite = maze_parts[4][5]; break;
                    case 'F': sprite = maze_parts[9][0]; break;
                    case 'G': sprite = maze_parts[9][1]; break;
                    case 'H': sprite = maze_parts[9][5]; break;
                    case 'I': sprite = maze_parts[9][22]; break;
                    case 'J': sprite = maze_parts[9][27]; break;
                    case 'K': sprite = maze_parts[12][10]; break;
                    case 'L': sprite = maze_parts[12][13]; break;
                    case 'M': sprite = maze_parts[12][17]; break;
                    case 'N': sprite = maze_parts[13][5]; break;
                    case 'O': sprite = maze_parts[13][22]; break;
                    case 'P': sprite = maze_parts[12][11]; break;
                    case 'Q': sprite = maze_parts[16][10]; break;
                    case 'R': sprite = maze_parts[16][17]; break;
                    case 'S': sprite = maze_parts[13][10]; break;
                    case 'T': sprite = maze_parts[13][17]; break;
                    case 'U': sprite = maze_parts[16][11]; break;
                }
               render_sprite(graphics_context, &sprite, x*8*zoom, 100+y*8*zoom, 0, zoom);
            }
        }

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
