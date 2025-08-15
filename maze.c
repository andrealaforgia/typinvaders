#include "maze.h"

#include <stdlib.h>
#include <string.h>

#include "geometry.h"
#include "render.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// clang-format off
const char *DEFAULT_MAZE_SYMBOLS[] = {
    "0111111111111231111111111114",
    "5............67............8",
    "5.9AAB.9AAAB.67.9AAAB.9AAB.8",
    "5*6  7.6   7.67.6   7.6  7*8",
    "5.CDDE.CDDDE.CE.CDDDE.CDDE.8",
    "5..........................8",
    "5.9AAB.9B.9AAAAAAB.9B.9AAB.8",
    "5.CDDE.67.CDDFGDDE.67.CDDE.8",
    "5......67....67....67......8",
    "HIIIIJ.6KAAB 67 9AAL7.MIIIIN",
    "     5.6GDDE CE CDDF7.8     ",
    "     5.67          67.8     ",
    "     5.67 OPPQQPPR 67.8     ",
    "11111S.CE 8      5 CE.T11111",
    "      .   8      5   .      ",
    "IIIIIJ.9B 8      5 9B.MIIIII",
    "     5.67 U111111V 67.8     ",
    "     5.67          67.8     ",
    "     5.67 9AAAAAAB 67.8     ",
    "01111S.CE CDDFGDDE CE.T11114",
    "5............67............8",
    "5.9AAB.9AAAB.67.9AAAB.9AAB.8",
    "5.CDF7.CDDDE.CE.CDDDE.6GDE.8",
    "5*..67.......  .......67..*8",
    "WAB.67.9B.9AAAAAAB.9B.67.9AX",
    "YDE.CE.67.CDDFGDDE.67.CE.CDZ",
    "5......67....67....67......8",
    "5.9AAAALKAAB.67.9AALKAAAAB.8",
    "5.CDDDDDDDDE.CE.CDDDDDDDDE.8",
    "5..........................8",
    "HIIIIIIIIIIIIIIIIIIIIIIIIIIN",
};
// clang-format on

maze_t create_maze(const sprite_sheet_ptr sprite_sheet,
                   const char **maze_symbols,
                   int maze_height) {
    maze_t maze;
    maze.width = (int)strlen(maze_symbols[0]);
    maze.height = maze_height;
    maze.symbols = maze_symbols;
    maze.parts = (sprite_t **)malloc(sizeof(sprite_t *) * maze.height);
    for (int y = 0; y < maze.height; y++) {
        maze.parts[y] = (sprite_t *)malloc(sizeof(sprite_t) * maze.width);
        for (int x = 0; x < maze.width; x++) {
            maze.parts[y][x] = create_sprite(sprite_sheet, x * 8, y * 8, 8, 8);
        }
    }
    return maze;
}

void render_maze(const graphics_context_ptr graphics_context,
                 const maze_ptr maze,
                 const rectangle_t maze_rectangle,
                 int zoom,
                 int show_power_pellets) {
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            char sym = maze->symbols[y][x];
            if (sym == ' ')
                continue;
            sprite_t sprite;
            int do_render = 1;
            switch (sym) {
            case '.':
                sprite = maze->parts[1][1];
                break;
            case '*':
                sprite = maze->parts[3][1];
                do_render = show_power_pellets;
                break;
            case '0':
                sprite = maze->parts[0][0];
                break;
            case '1':
                sprite = maze->parts[0][1];
                break;
            case '2':
                sprite = maze->parts[0][13];
                break;
            case '3':
                sprite = maze->parts[0][14];
                break;
            case '4':
                sprite = maze->parts[0][27];
                break;
            case '5':
                sprite = maze->parts[1][0];
                break;
            case '6':
                sprite = maze->parts[1][13];
                break;
            case '7':
                sprite = maze->parts[1][14];
                break;
            case '8':
                sprite = maze->parts[1][27];
                break;
            case '9':
                sprite = maze->parts[2][2];
                break;
            case 'A':
                sprite = maze->parts[2][3];
                break;
            case 'B':
                sprite = maze->parts[2][5];
                break;
            case 'C':
                sprite = maze->parts[4][2];
                break;
            case 'D':
                sprite = maze->parts[4][3];
                break;
            case 'E':
                sprite = maze->parts[4][5];
                break;
            case 'F':
                sprite = maze->parts[7][13];
                break;
            case 'G':
                sprite = maze->parts[7][14];
                break;
            case 'H':
                sprite = maze->parts[9][0];
                break;
            case 'I':
                sprite = maze->parts[9][1];
                break;
            case 'J':
                sprite = maze->parts[9][5];
                break;
            case 'K':
                sprite = maze->parts[9][8];
                break;
            case 'L':
                sprite = maze->parts[9][19];
                break;
            case 'M':
                sprite = maze->parts[9][22];
                break;
            case 'N':
                sprite = maze->parts[9][27];
                break;
            case 'O':
                sprite = maze->parts[12][10];
                break;
            case 'P':
                sprite = maze->parts[12][11];
                break;
            case 'Q':
                sprite = maze->parts[12][13];
                break;
            case 'R':
                sprite = maze->parts[12][17];
                break;
            case 'S':
                sprite = maze->parts[13][5];
                break;
            case 'T':
                sprite = maze->parts[13][23];
                break;
            case 'U':
                sprite = maze->parts[16][10];
                break;
            case 'V':
                sprite = maze->parts[16][17];
                break;
            case 'W':
                sprite = maze->parts[24][0];
                break;
            case 'X':
                sprite = maze->parts[24][27];
                break;
            case 'Y':
                sprite = maze->parts[25][0];
                break;
            case 'Z':
                sprite = maze->parts[25][27];
                break;
            default:
                continue;
            }
            if (do_render) {
                render_sprite(graphics_context,
                              &sprite,
                              maze_rectangle.top_left.x + x * 8 * zoom,
                              maze_rectangle.top_left.y + y * 8 * zoom,
                              0,
                              zoom);
            }
        }
    }
}

void destroy_maze(maze_ptr maze) {
    if (!maze || !maze->parts)
        return;
    for (int y = 0; y < maze->height; y++)
        free(maze->parts[y]);
    free(maze->parts);
    maze->parts = NULL;
}

int calculate_maze_zoom(const graphics_context_ptr graphics_context,
                        const maze_ptr maze) {
    int maze_sprite_height = maze->height * 8;
    int maze_sprite_width = maze->width * 8;
    int maze_h_scale = graphics_context->screen_width / maze_sprite_width;
    int maze_v_scale = graphics_context->screen_height / maze_sprite_height;
    return MIN(maze_h_scale, maze_v_scale);
}

rectangle_t create_maze_rectangle(const graphics_context_ptr graphics_context,
                                  int zoom,
                                  const maze_ptr maze) {
    int maze_sprite_height = maze->height * 8;
    int maze_sprite_width = maze->width * 8;
    int maze_scaled_width = maze_sprite_width * zoom;
    int maze_scaled_height = maze_sprite_height * zoom;
    return rectangle(
        point(graphics_context->screen_width / 2 - maze_scaled_width / 2,
              graphics_context->screen_height / 2 - maze_scaled_height / 2),
        maze_scaled_width,
        maze_scaled_height);
}
