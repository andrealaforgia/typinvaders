#ifndef MAZE_H_
#define MAZE_H_

#include "graphics.h"
#include "sprite.h"

#define MAZE_SPRITE_COUNT 44

extern const char *DEFAULT_MAZE_SYMBOLS[];

typedef struct {
    int width;
    int height;
    sprite_t **parts;
    const char **symbols;
} maze_t, *maze_ptr;

maze_t create_maze(const sprite_sheet_ptr sprite_sheet,
                   const char **maze_symbols,
                   int maze_height);

void render_maze(const graphics_context_ptr graphics_context,
                 const maze_ptr maze,
                 const rectangle_t maze_rectangle,
                 int zoom);

void destroy_maze(maze_ptr maze);

int calculate_maze_zoom(const graphics_context_ptr graphics_context,
                        const maze_ptr maze);

rectangle_t create_maze_rectangle(const graphics_context_ptr graphics_context,
                                  int zoom,
                                  const maze_ptr maze);

#endif  // MAZE_H_
