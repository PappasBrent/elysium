#include "color.h"
#include "types.h"
#include <ncurses.h>

int color_pair_index(u8 fg, u8 bg) {
        return 1 + (fg * NUM_COLORS + bg);
}

void init_color_pairs(void) {
        u8 fg, bg;

        /* Redefine standard colors. This may not always work.
         * See https://stackoverflow.com/a/70480456/6824430. */
        init_color(COLOR_BLACK, 0, 0, 0);
        init_color(COLOR_RED, 1000, 0, 0);
        init_color(COLOR_GREEN, 0, 500, 0);
        init_color(COLOR_YELLOW, 800, 750, 000);
        init_color(COLOR_BLUE, 0, 0, 1000);
        init_color(COLOR_MAGENTA, 1000, 0, 1000);
        init_color(COLOR_CYAN, 0, 1000, 1000);
        init_color(COLOR_WHITE, 1000, 1000, 1000);

        /* NOTE(bpp): ncurses provides eight color pairs. This means there are a
        total of 8 * 8 = 64 possible color pairs. We initialize all of them
        up-front, and use helper macros to access them later. */
        for (fg = 0; fg < NUM_COLORS; fg++)
                for (bg = 0; bg < NUM_COLORS; bg++)
                        init_pair(color_pair_index(fg, bg), fg, bg);
}
