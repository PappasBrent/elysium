#ifndef ELYSIUM_GAME_H
#define ELYSIUM_GAME_H

#include "constants.h"
#include "entity.h"
#include "menu.h"
#include "tile.h"
#include "types.h"

typedef boolean(input_handler)(int);
typedef void(draw_handler)(void);
typedef void(update_handler)(void);

enum game_state { LEVEL, TITLE, PAUSE, DIALOGUE, N_GAME_STATES };

enum flags { QUIT, HAS_SURFBOARD, HAS_HIBISCUS, N_FLAGS };

struct game_t {
        /* The current game state */
        enum game_state state;

        /* Terrain kinds */
        enum terrain_kind terrain_kinds[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Terrain colors */
        u8 terrain_colors[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Entity kinds */
        enum entity_kind entity_kinds[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Entity colors. Also the color of keys and doors. */
        u8 entity_colors[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* NPC ids */
        enum npc_id npc_ids[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Menu body ids for NPCs and signs */
        enum menu_body_id menu_body_ids[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Destination row for portals */
        u8 portal_dest_rows[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];
        /* Destination col for portals */
        u8 portal_dest_cols[WORLD_HEIGHT_ROWS][WORLD_WIDTH_COLS];

        /* Bit vector of boolean flags */
        u8 flags;

        /* Bit vector of obtained keys. One key for each color. */
        u8 keys;

        /* Bit vector of NPCs the player has convinced to come to the Field.
           1 means yes, 0 means no */
        u32 convinced_nps;

        /* Array of currently available menu options */
        enum menu_option_id
                available_menu_option_ids[N_MAX_AVAILABLE_MENU_OPTIONS];

        /* The row of the currently speaking NPC's ID */
        u8 speaking_npc_row;
        /* The column of the currently speaking NPC's ID */
        u8 speaking_npc_col;

        /* The index of the currently visible menu body */
        enum menu_body_id menu_body_id;
        /* Number of currently available menu text options */
        u8 n_available_menu_option_ids;

        /* The index of the currently focused option */
        u8 i_focused_option;

        /* The row the player is at */
        u8 player_row;
        /* The col the player is at */
        u8 player_col;

        /* The row that the NPC with the given ID is on. */
        u8 npc_rows[26];
        /* The col that the NPC with the given ID is on. */
        u8 npc_cols[26];

        /* The last 7 rows the player stepped on, excluding the current row. */
        u8 last_7_rows[7];
        /* The last 7 cols the player stepped on, excluding the current col. */
        u8 last_7_cols[7];
        /* The last position in the ring buffer. */
        u8 last_7_ring_end;

        /* Whether NPC D's quest has been started and not yet finished. */
        boolean npc_d_quest_active;
};

boolean flag(u8 flag);
void turn_on_flag(u8 flag);
void turn_off_flag(u8 flag);

boolean game_has_key(u8 color);
void game_get_key(u8 color);

boolean has_convinced_npc(u8 npc_id);
void convince_npc(u8 npc_id);
void set_speaker_menu_id(u8 menu_id);
void convince_speaker(void);
void set_npc_row_col(u8 npc_id, u8 row, u8 col);
u8 npc_row(u8 npc_id);
u8 npc_col(u8 npc_id);

/* Global game object */
extern struct game_t game;

/* Initializes the game and window */
extern void init_game(void);
/* Tears down the game and window */
extern void end_game(void);

/* Handles player input. Returns true if the game needs to be updated and
redrawn afterwards. */
extern input_handler handle_input;
/* Draw the game */
extern draw_handler draw_game;
/* Update the game */
extern update_handler update_game;
/* Game state entrance function */
extern void game_t_enter_state(enum game_state);

#endif
