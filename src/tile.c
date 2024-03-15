#include "tile.h"
#include <ncurses.h>

char terrain_glyphs[N_TERRAIN_KINDS] = {
        ',', /* GRASS, */
        '.', /* STONE, */
        '-', /* WATER_LAKE, */
        '~', /* WATER_OCEAN, */
        '.', /* SAND, */
        '~' /* LAVA, */
};
