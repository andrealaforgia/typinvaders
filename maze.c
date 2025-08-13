#include "maze.h"

#include <string.h>

#include "geometry.h"
#include "render.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// clang-format off
const char* maze_symbols[] = {
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

maze_t create_maze(const sprite_sheet_ptr sprite_sheet) {
    maze_t maze;
    int maze_x = 28 * 9;
    int maze_y = 0;
    for (int i = 0; i < 44; i++) {
        maze.parts[i] = create_sprite(sprite_sheet, maze_x, maze_y, 8, 8);
        maze_x += 8;
    }
    return maze;
}

void render_maze(const graphics_context_ptr graphics_context,
                 const maze_t *maze,
                 const rectangle_t maze_rectangle,
                 int zoom) {
    int maze_width = strlen(maze_symbols[0]);
    int maze_height = sizeof(maze_symbols) / sizeof(char *);

    for (int x = 0; x < maze_width; x++) {
        for (int y = 0; y < maze_height; y++) {
            sprite_t sprite;
            switch (maze_symbols[y][x]) {
            case ' ':
                continue;
            case '.':
                sprite = maze->parts[1];
                break;
            case '*':
                sprite = maze->parts[3];
                break;
            case '0':
                sprite = maze->parts[0];
                break;
            case '1':
                sprite = maze->parts[1];
                break;
            case '2':
                sprite = maze->parts[13];
                break;
            case '3':
                sprite = maze->parts[14];
                break;
            case '4':
                sprite = maze->parts[27];
                break;
            case '5':
                sprite = maze->parts[0];
                break;
            case '6':
                sprite = maze->parts[13];
                break;
            case '7':
                sprite = maze->parts[14];
                break;
            case '8':
                sprite = maze->parts[27];
                break;
            case '9':
                sprite = maze->parts[2];
                break;
            case 'A':
                sprite = maze->parts[3];
                break;
            case 'B':
                sprite = maze->parts[5];
                break;
            case 'C':
                sprite = maze->parts[4];
                break;
            case 'D':
                sprite = maze->parts[3];
                break;
            case 'E':
                sprite = maze->parts[5];
                break;
            case 'F':
                sprite = maze->parts[7];
                break;
            case 'G':
                sprite = maze->parts[14];
                break;
            case 'H':
                sprite = maze->parts[9];
                break;
            case 'I':
                sprite = maze->parts[1];
                break;
            case 'J':
                sprite = maze->parts[5];
                break;
            case 'K':
                sprite = maze->parts[8];
                break;
            case 'L':
                sprite = maze->parts[19];
                break;
            case 'M':
                sprite = maze->parts[22];
                break;
            case 'N':
                sprite = maze->parts[27];
                break;
            case 'O':
                sprite = maze->parts[10];
                break;
            case 'P':
                sprite = maze->parts[11];
                break;
            case 'Q':
                sprite = maze->parts[13];
                break;
            case 'R':
                sprite = maze->parts[17];
                break;
            case 'S':
                sprite = maze->parts[5];
                break;
            case 'T':
                sprite = maze->parts[22];
                break;
            case 'U':
                sprite = maze->parts[10];
                break;
            case 'V':
                sprite = maze->parts[17];
                break;
            case 'W':
                sprite = maze->parts[0];
                break;
            case 'X':
                sprite = maze->parts[27];
                break;
            case 'Y':
                sprite = maze->parts[0];
                break;
            case 'Z':
                sprite = maze->parts[27];
                break;
            }
            render_sprite(graphics_context,
                          &sprite,
                          maze_rectangle.top_left.x + x * 8 * zoom,
                          maze_rectangle.top_left.y + y * 8 * zoom,
                          0,
                          zoom);
        }
    }
}

void get_maze_dimensions(int *width, int *height) {
    *width = strlen(maze_symbols[0]);
    *height = sizeof(maze_symbols) / sizeof(char *);
}

int calculate_maze_zoom(const graphics_context_ptr graphics_context) {
    int maze_width, maze_height;
    get_maze_dimensions(&maze_width, &maze_height);

    int maze_sprite_height = maze_height * 8;
    int maze_sprite_width = maze_width * 8;

    int maze_h_scale = graphics_context->screen_width / maze_sprite_width;
    int maze_v_scale = graphics_context->screen_height / maze_sprite_height;

    return MIN(maze_h_scale, maze_v_scale);
}

rectangle_t create_maze_rectangle(const graphics_context_ptr graphics_context,
                                  int zoom) {
    int maze_width, maze_height;
    get_maze_dimensions(&maze_width, &maze_height);

    int maze_sprite_height = maze_height * 8;
    int maze_sprite_width = maze_width * 8;

    int maze_scaled_width = maze_sprite_width * zoom;
    int maze_scaled_height = maze_sprite_height * zoom;

    return rectangle(
        point(graphics_context->screen_width / 2 - maze_scaled_width / 2,
              graphics_context->screen_height / 2 - maze_scaled_height / 2),
        maze_scaled_width,
        maze_scaled_height);
}
