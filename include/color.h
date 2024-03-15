#ifndef ELYSIUM_COLORS_H
#define ELYSIUM_COLORS_H

#include "types.h"

static const u8 NUM_COLORS = 8;

int color_pair_index(u8 fg, u8 bg);

/* Initialize all possible color pairs */
extern void init_color_pairs(void);

#endif
