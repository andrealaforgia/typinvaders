#include "font.h"

#include "render.h"

static sprite_t font[256] = { 0 };

void load_font_from_sprite_sheet(const sprite_sheet_ptr sprite_sheet) {
    const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!0123456789-";
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

void write(const graphics_context_ptr graphics_context,
           char *text,
           int x,
           int y,
           int zoom) {
    int char_x = x;
    for (int i = 0; text[i]; i++) {
        if (text[i] == ' ') {
            char_x += 4 * zoom;
            continue;
        }
        render_sprite(
            graphics_context, &font[(int)(text[i])], char_x, y, 0, zoom);
        char_x += 8 * zoom;
    }
}
