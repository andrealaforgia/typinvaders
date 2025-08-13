#ifndef MAZE_H_
#define MAZE_H_

#include "graphics.h"
#include "sprite.h"

typedef struct maze_t {
    sprite_t parts[44];
    sprite_t layout[31][28];
} maze_t;

/**
 * Creates a maze structure with all the maze parts
 * @param sprite_sheet Pointer to the sprite sheet containing maze parts
 * @return maze_t structure with initialized maze parts
 */
maze_t create_maze(const sprite_sheet_ptr sprite_sheet);

/**
 * Renders the maze on screen
 * @param graphics_context Pointer to the graphics context
 * @param maze Pointer to the maze structure
 * @param maze_rectangle Rectangle defining the maze position and size
 * @param zoom Zoom factor for maze scaling
 */
void render_maze(const graphics_context_ptr graphics_context,
                 const maze_t *maze,
                 const rectangle_t maze_rectangle,
                 int zoom);

/**
 * Gets the maze dimensions
 * @param width Pointer to store maze width
 * @param height Pointer to store maze height
 */
void get_maze_dimensions(int *width, int *height);

/**
 * Calculates the optimal zoom factor for the maze to fit the screen
 * @param graphics_context Pointer to the graphics context
 * @return Optimal zoom factor
 */
int calculate_maze_zoom(const graphics_context_ptr graphics_context);

/**
 * Creates a rectangle for the maze centered on screen
 * @param graphics_context Pointer to the graphics context
 * @param zoom Zoom factor for maze scaling
 * @return Rectangle defining maze position and size
 */
rectangle_t create_maze_rectangle(const graphics_context_ptr graphics_context,
                                  int zoom);

#endif  // MAZE_H_
