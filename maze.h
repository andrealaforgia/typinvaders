#ifndef MAZE_H_
#define MAZE_H_

#include "graphics.h"
#include "sprite.h"

typedef struct maze_t {
    sprite_t parts[44];
    sprite_t layout[31][28];
} maze_t, *maze_ptr;

maze_t create_maze(const sprite_sheet_ptr sprite_sheet);

void render_maze(const graphics_context_ptr graphics_context,
                 const maze_ptr maze,
                 const rectangle_t maze_rectangle,
                 int zoom);

void get_maze_dimensions(int *width, int *height);

int calculate_maze_zoom(const graphics_context_ptr graphics_context);

rectangle_t create_maze_rectangle(const graphics_context_ptr graphics_context,
                                  int zoom);

#endif  // MAZE_H_
