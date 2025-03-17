#include "text.h"

#include <assert.h>
#include <stdio.h>

#include "coords.h"
#include "graphics.h"
#include "inline.h"

const coords_t FONT_COORDS[] = {
    // "A"
    {0.0000, 2.0000, 7},
    {1.0000, 1.0000, 7},
    {1.0000, -1.0000, 7},
    {0.0000, -2.0000, 7},
    {-2.0000, 1.0000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, -1.0000, 0},

    // "B"
    {0.0000, 3.0000, 7},
    {1.5000, 0.0000, 7},
    {0.5000, -0.5000, 7},
    {0.0000, -0.5000, 7},
    {-0.5000, -0.5000, 7},
    {-1.5000, 0.0000, 7},
    {1.5000, 0.0000, 0},
    {0.5000, -0.5000, 7},
    {0.0000, -0.5000, 7},
    {-0.5000, -0.5000, 7},
    {-1.5000, 0.0000, 7},
    {3.0000, 0.0000, 0},

    // "C"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {-2.0000, -3.0000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, 0.0000, 0},

    // "D"
    {0.0000, 3.0000, 7},
    {1.0000, 0.0000, 7},
    {1.0000, -1.0000, 7},
    {0.0000, -1.0000, 7},
    {-1.0000, -1.0000, 7},
    {-1.0000, 0.0000, 7},
    {3.0000, 0.0000, 0},

    // "E"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {-0.5000, -1.5000, 0},
    {-1.5000, 0.0000, 7},
    {0.0000, -1.5000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, 0.0000, 0},

    // "F"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {-0.5000, -1.5000, 0},
    {-1.5000, 0.0000, 7},
    {0.0000, -1.5000, 0},
    {3.0000, 0.0000, 0},

    // "G"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, -1.0000, 7},
    {-1.0000, -1.0000, 0},
    {1.0000, 0.0000, 7},
    {0.0000, -1.0000, 7},
    {-2.0000, 0.0000, 7},
    {3.0000, 0.0000, 0},

    // "H"
    {0.0000, 3.0000, 7},
    {0.0000, -1.5000, 0},
    {2.0000, 0.0000, 7},
    {0.0000, 1.5000, 0},
    {0.0000, -3.0000, 7},
    {1.0000, 0.0000, 0},

    // "I"
    {2.0000, 0.0000, 7},
    {-1.0000, 0.0000, 0},
    {0.0000, 3.0000, 7},
    {1.0000, 0.0000, 0},
    {-2.0000, 0.0000, 7},
    {3.0000, -3.0000, 0},

    // "J"
    {0.0000, 1.0000, 0},
    {1.0000, -1.0000, 7},
    {1.0000, 0.0000, 7},
    {0.0000, 3.0000, 7},
    {1.0000, -3.0000, 0},

    // "K"
    {0.0000, 3.0000, 7},
    {1.5000, 0.0000, 0},
    {-1.5000, -1.5000, 7},
    {1.5000, -1.5000, 7},
    {1.5000, 0.0000, 0},

    // "L"
    {0.0000, 3.0000, 0},
    {0.0000, -3.0000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, 0.0000, 0},

    // "M"
    {0.0000, 3.0000, 7},
    {1.0000, -1.0000, 7},
    {1.0000, 1.0000, 7},
    {0.0000, -3.0000, 7},
    {1.0000, 0.0000, 0},

    // "N"
    {0.0000, 3.0000, 7},
    {2.0000, -3.0000, 7},
    {0.0000, 3.0000, 7},
    {1.0000, -3.0000, 0},

    // "O"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, -3.0000, 7},
    {-2.0000, 0.0000, 7},
    {3.0000, 0.0000, 0},

    // "P"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {-2.0000, 0.0000, 7},
    {1.5000, -1.5000, 0},
    {1.5000, 0.0000, 0},

    // "Q"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, -2.0000, 7},
    {-1.0000, -1.0000, 7},
    {-1.0000, 0.0000, 7},
    {1.0000, 1.0000, 0},
    {1.0000, -1.0000, 7},
    {1.0000, 0.0000, 0},

    // "R"
    {0.0000, 3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {-2.0000, 0.0000, 7},
    {0.5000, 0.0000, 0},
    {1.5000, -1.5000, 7},
    {1.0000, 0.0000, 0},

    // "S"
    {2.0000, 0.0000, 7},
    {0.0000, 1.5000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, 1.5000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, -3.0000, 0},

    // "T"
    {1.0000, 0.0000, 0},
    {0.0000, 3.0000, 7},
    {-1.0000, 0.0000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, -3.0000, 0},

    // "U"
    {0.0000, 3.0000, 0},
    {0.0000, -3.0000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, 3.0000, 7},
    {1.0000, -3.0000, 0},

    // "V"
    {0.0000, 3.0000, 0},
    {1.0000, -3.0000, 7},
    {1.0000, 3.0000, 7},
    {1.0000, -3.0000, 0},

    // "W"
    {0.0000, 3.0000, 0},
    {0.0000, -3.0000, 7},
    {1.0000, 1.0000, 7},
    {1.0000, -1.0000, 7},
    {0.0000, 3.0000, 7},
    {1.0000, -3.0000, 0},

    // "X"
    {2.0000, 3.0000, 7},
    {-2.0000, 0.0000, 0},
    {2.0000, -3.0000, 7},
    {1.0000, 0.0000, 0},

    // "Y"
    {1.0000, 0.0000, 0},
    {0.0000, 2.0000, 7},
    {-1.0000, 1.0000, 7},
    {2.0000, 0.0000, 0},
    {-1.0000, -1.0000, 7},
    {2.0000, -2.0000, 0},

    // "Z"
    {0.0000, 3.0000, 0},
    {2.0000, 0.0000, 7},
    {-2.0000, -3.0000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, 0.0000, 0},

    // SPACE
    {3.0000, 0.0000, 0},

    // "1"
    {1.0000, 0.0000, 0},
    {0.0000, 3.0000, 7},
    {2.0000, -3.0000, 0},

    // "2"
    {0.0000, 3.0000, 0},
    {2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, 0.0000, 0},

    // "3"
    {2.0000, 0.0000, 7},
    {0.0000, 3.0000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, -1.5000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, -1.5000, 0},

    // "4"
    {0.0000, 3.0000, 0},
    {0.0000, -1.5000, 7},
    {2.0000, 0.0000, 7},
    {0.0000, 1.5000, 0},
    {0.0000, -3.0000, 7},
    {1.0000, 0.0000, 0},

    // "5"
    {2.0000, 0.0000, 7},
    {0.0000, 1.5000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, 1.5000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, -3.0000, 0},

    // "6"
    {0.0000, 1.5000, 0},
    {2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, 3.0000, 7},
    {3.0000, -3.0000, 0},

    // "7"
    {0.0000, 3.0000, 0},
    {2.0000, 0.0000, 7},
    {0.0000, -3.0000, 7},
    {1.0000, 0.0000, 0},

    // "8"
    {2.0000, 0.0000, 7},
    {0.0000, 3.0000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, -3.0000, 7},
    {0.0000, 1.5000, 0},
    {2.0000, 0.0000, 7},
    {1.0000, -1.5000, 0},

    // "9"
    {2.0000, 0.0000, 0},
    {0.0000, 3.0000, 7},
    {-2.0000, 0.0000, 7},
    {0.0000, -1.5000, 7},
    {2.0000, 0.0000, 7},
    {1.0000, -1.5000, 0},

    // (c)
    {0.0000, 0.5000, 0},
    {1.5000, -0.5000, 7},
    {1.5000, 0.5000, 7},
    {0.0000, 2.0000, 7},
    {-1.5000, 0.5000, 7},
    {-1.5000, -0.5000, 7},
    {0.0000, -2.0000, 7},
    {2.0000, 1.5000, 0},
    {-1.0000, 0.0000, 7},
    {0.0000, -1.0000, 7},
    {1.0000, 0.0000, 7},
    {2.0000, -1.0000, 0}};

const bounds_t LETTER_BOUNDS[] = {
    {0, 7},     {7, 19},    {19, 24},   {24, 31},   {31, 38},   {38, 44},
    {44, 52},   {52, 58},   {58, 64},   {64, 69},   {69, 74},   {74, 78},
    {78, 83},   {83, 87},   {87, 92},   {92, 98},   {98, 106},  {106, 113},
    {113, 119}, {119, 124}, {124, 129}, {129, 133}, {133, 139}, {139, 143},
    {143, 149}, {149, 154}};

const bounds_t SPACE_BOUNDS = {154, 155};

const bounds_t DIGIT_BOUNDS[] = {{87, 92},   {155, 158}, {158, 165}, {165, 171},
                                 {171, 177}, {177, 183}, {183, 189}, {189, 193},
                                 {193, 200}, {200, 206}};

const bounds_t COPYRIGHT_COORDS = {206, 218};

ALWAYS_INLINE text_dimensions_t text_dimensions(int width, int height) {
  text_dimensions_t text_dim;
  text_dim.width = width;
  text_dim.height = height;
  return text_dim;
}

ALWAYS_INLINE bounds_t bounds_for_char(char c) {
  bounds_t bounds = {0, 0};
  if (c == 'c') {
    bounds = COPYRIGHT_COORDS;
  } else if (c == ' ') {
    bounds = SPACE_BOUNDS;
  } else if (c >= 'A' && c <= 'Z') {
    bounds = LETTER_BOUNDS[c - 'A'];
  } else if (c >= '0' && c <= '9') {
    bounds = DIGIT_BOUNDS[c - '0'];
  }
  return bounds;
}

ALWAYS_INLINE text_dimensions_t calculate_text_dimensions(const char *s,
                                                          int scale) {
  double min_y = 999999;
  double max_x = 0;
  double cx = 0;
  double cy = 0;
  for (size_t i = 0; s[i]; i++) {
    bounds_t bounds = bounds_for_char(s[i]);
    if (bounds.upper != 0) {
      for (int j = bounds.lower; j < bounds.upper; j++) {
        cx += FONT_COORDS[j].x_delta * scale;
        cy -= FONT_COORDS[j].y_delta * scale;
        if (cy <= min_y) {
          min_y = cy;
        }
        if (cx > max_x) {
          max_x = cx;
        }
      }
    }
  }
  return text_dimensions(max_x, -min_y);
}

ALWAYS_INLINE point_t write_text(const graphics_context_ptr graphics_context,
                                 const char *s, const point_t position,
                                 int scale, color_t color) {
  double cx = position.x;
  double cy = position.y;
  for (size_t i = 0; s[i]; i++) {
    bounds_t bounds = bounds_for_char(s[i]);
    if (bounds.upper != 0) {
      for (int j = bounds.lower; j < bounds.upper; j++) {
        double nx = cx + (FONT_COORDS[j].x_delta * scale);
        double ny = cy - (FONT_COORDS[j].y_delta * scale);
        if (FONT_COORDS[j].brightness > 0) {
          draw_line(graphics_context, cx, cy, nx, ny, color);
        }
        cx = nx;
        cy = ny;
      }
    }
  }
  return point(cx, cy);
}

ALWAYS_INLINE point_t write_number(const graphics_context_ptr graphics_context,
                                   const point_t position, int number,
                                   int scale) {
  char str[10] = {0};
  snprintf(str, sizeof str, "%09d", number);
  return write_text(graphics_context, str, position, scale, COLOR_YELLOW);
}
