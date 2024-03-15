#ifndef ELYSIUM_TILE_H
#define ELYSIUM_TILE_H

enum terrain_kind {
        GRASS,
        STONE,
        WATER_LAKE,
        WATER_OCEAN,
        SAND,
        LAVA,
        N_TERRAIN_KINDS
};

extern char terrain_glyphs[N_TERRAIN_KINDS];

#endif

