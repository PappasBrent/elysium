#ifndef ELYSIUM_CONSTANTS
#define ELYSIUM_CONSTANTS

enum constants {
        /* Rows */

        WORLD_HEIGHT_ROWS = 80,
        SCENE_HEIGHT_ROWS = 16,
        WORLD_HEIGHT_SCENES = WORLD_HEIGHT_ROWS / SCENE_HEIGHT_ROWS,
        TERM_HEIGHT_ROWS = SCENE_HEIGHT_ROWS + 4,

        /* Columns */

        WORLD_WIDTH_COLS = 240,
        SCENE_WIDTH_COLS = 48,
        WORLD_WIDTH_SCENES = WORLD_WIDTH_COLS / SCENE_WIDTH_COLS,
        TERM_WIDTH_COLS = SCENE_WIDTH_COLS,

        /* Menu margins */

        MENU_LEFT = 2,
        MENU_RIGHT = SCENE_WIDTH_COLS - 1 - MENU_LEFT,
        MENU_TOP = 2,
        MENU_BOT = SCENE_HEIGHT_ROWS - MENU_TOP - 1,

        /* Menu margins */

        MENU_WIDTH = MENU_RIGHT - MENU_LEFT + 1,
        MENU_HEIGHT = MENU_BOT - MENU_TOP + 1,
        N_MAX_AVAILABLE_MENU_OPTIONS = 3,

        /* Max menu option text length. Minus 2 for padding, minus 3 for option
    prefix "x. " */
        MAX_MENU_OPTION_TEXT_LEN = MENU_WIDTH - 2 - 3,

        /* Max length of a single line of menu body text. Minus 2 for padding */
        MAX_MENU_BODY_TEXT_WIDTH = MENU_WIDTH - 2,

        /* Max length of entire menu text body */
        MAX_MENU_BODY_TEXT_LEN =
                MAX_MENU_BODY_TEXT_WIDTH *
                (MENU_HEIGHT - N_MAX_AVAILABLE_MENU_OPTIONS - 2)
};

#endif
