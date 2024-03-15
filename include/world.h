#ifndef ELYSIUM_SCENE_H
#define ELYSIUM_SCENE_H

#include "constants.h"

/* Conceptually, the world is a grid of scenes, where each scene is in turn a
grid of tiles. In practice, we implement the entire world as a single matrix of
tiles, instead of a matrix scenes. When drawing the world, we just have to do a
bit of math to determine where the top-left corner of the current scene is. */

extern char base_world[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
extern char ending_world[SCENE_HEIGHT_ROWS][SCENE_WIDTH_COLS];

#endif
