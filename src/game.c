#include "game.h"
#include "assert.h"
#include "color.h"
#include "constants.h"
#include "entity.h"
#include "menu.h"
#include "menu_manual.h"
#include "ncurses.h"
#include "printw_wrap.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void(state_entrance_handler)(void);
typedef boolean(player_interaction_handler)(u8, u8);

/* Set up and tear down the game */

void init_game(void);
void end_game(void);

/* Title option commands */

/* Input handlers */

input_handler handle_input;
input_handler handle_input_world;
input_handler handle_input_menu;

/* Draw handlers */

draw_handler draw_game;
draw_handler draw_world;
draw_handler draw_menu;

update_handler update_game;

/* Player interaction handlers */

player_interaction_handler handle_player_interaction;

/* Helper functions */

void draw_menu_and_menu_body(enum menu_body_id mbi);
void print_menu_option(u8 i);
void player_step_on(u8 d_row, u8 d_col);
void draw_tile(u8 row, u8 col, u8 drow, u8 dcol);
boolean is_water(enum terrain_kind tk);
void append_player_pos_to_ring(void);
boolean compare_previous_i_row_and_column(u8 i, u8 row, u8 col);
boolean check_npc_d_quest(void);
int wrapping_sub(int min, int max, int n, int i);

static u8 sub1_wrap(u8 x, u8 r) {
        return ((x == 0) * r) + ((x > 0) * x) - 1;
}

static u8 add1_wrap(u8 x, u8 r) {
        return (x + 1) % r;
}

static u8 cur_scene_top_row(u8 r) {
        return (r / SCENE_HEIGHT_ROWS * SCENE_HEIGHT_ROWS);
}

static u8 cur_scene_left_col(u8 c) {
        return (c / SCENE_WIDTH_COLS * SCENE_WIDTH_COLS);
}

/* Starts the game. Operations should appear in reverse order that they appear
in end_game() */
void init_game(void) {
        /* Start ncurses mode */
        initscr();
        /* Disable line buffering */
        raw();
        /* Get arrow keys, etc. */
        keypad(stdscr, TRUE);
        /* Don't show chars when taking input */
        noecho();
        /* Hide cursor */
        curs_set(0);
        /* Check for colors */
        if (has_colors() == FALSE) {
                printf("Your terminal does not support color\n");
                end_game();
                exit(1);
        }
        /* Use colors */
        start_color();
        /* Set up colors pairs */
        init_color_pairs();

        /* Initialize game flags */
        game.flags = 0;
        game.keys = 0;
        game.npc_d_quest_active = 0;
        game.last_7_ring_end = 6;

        open_title_menu();

        /* Start the game at the title menu */
        game_t_enter_state(TITLE);
}

void end_game(void) {
        /* Show cursor */
        curs_set(1);
        /* Show chars when taking input */
        echo();
        /* Turn off keypad */
        keypad(stdscr, FALSE);
        /* Enable line buffering */
        noraw();
        /* End ncurses mode */
        endwin();
}

boolean handle_input(int ch) {
        /* Handle input based on game state */
        switch (game.state) {
        case LEVEL:
                return handle_input_world(ch);
        case TITLE:
        case PAUSE:
        case DIALOGUE:
                return handle_input_menu(ch);

        case N_GAME_STATES:
                break;
        }
        return TRUE;
}

boolean handle_input_world(int ch) {
        u8 r = game.player_row, c = game.player_col, next_r, next_c;
        switch (ch) {
        case 'q':
                turn_on_flag(QUIT);
                break;
        case 'p':
                pause_game();
                break;
        case 'w':
        case KEY_UP:
                next_r = sub1_wrap(r, WORLD_HEIGHT_ROWS);
                return handle_player_interaction(next_r, c);
        case 'a':
        case KEY_LEFT:
                next_c = sub1_wrap(c, WORLD_WIDTH_COLS);
                return handle_player_interaction(r, next_c);
        case 's':
        case KEY_DOWN:
                next_r = add1_wrap(r, WORLD_HEIGHT_ROWS);
                return handle_player_interaction(next_r, c);
        case 'd':
        case KEY_RIGHT:
                next_c = add1_wrap(c, WORLD_WIDTH_COLS);
                return handle_player_interaction(r, next_c);
        default:
                return 0;
        }
        return 1;
}

boolean handle_input_menu(int ch) {
        u8 i = game.i_focused_option, wrap = game.n_available_menu_option_ids;
        switch (ch) {
        case 'q':
                turn_on_flag(QUIT);
                break;
        case 'p':
                if (game.state == PAUSE)
                        enter_state_level();
                break;
        case 'w':
        case KEY_UP:
                game.i_focused_option = sub1_wrap(i, wrap);
                break;
        case 's':
        case KEY_DOWN:
                game.i_focused_option = add1_wrap(i, wrap);
                break;
        case '\n':
                select_option(game.available_menu_option_ids[i]);
                break;
        default:
                return 0;
        }
        return 1;
}

void draw_game(void) {
        /* Clear the screen. Use erase() instead of clear() to prevent screen
        flickering. */
        erase();
        /* Draw based on game state */
        switch (game.state) {
        case LEVEL:
                draw_world();
                break;
        case TITLE:
        case PAUSE:
        case DIALOGUE:
                draw_menu();
                break;

        case N_GAME_STATES:
                break;
        }
        /* Refresh the screen to show updates */
        refresh();
}

void draw_world(void) {
        u8 prow = game.player_row, pcol = game.player_col,
           /* World offset row */
                wr = cur_scene_top_row(prow),
           /* World offset column */
                wc = cur_scene_left_col(pcol),
           /* Draw row */
                dr,
           /* Draw column */
                dc;

        int color_pair = color_pair_index(COLOR_RED, COLOR_BLACK),
            underline =
                    (A_UNDERLINE * (flag(HAS_SURFBOARD) &&
                                    is_water(game.terrain_kinds[prow][pcol])));
#ifdef DEBUG
        char buffer[MAX_MENU_OPTION_TEXT_LEN];
#endif

        for (dr = 0; dr < SCENE_HEIGHT_ROWS; dr++)
                for (dc = 0; dc < SCENE_WIDTH_COLS; dc++)
                        draw_tile(wr + dr, wc + dc, dr, dc);

        /* Draw the player on top of the world */
        mvaddch(prow - wr, pcol - wc, '@');
        mvchgat(prow - wr, pcol - wc, 1, A_BOLD | underline, color_pair, NULL);

        /* Draw the current sign, if any */
        if (game.entity_kinds[prow][pcol] == SIGN)
                draw_menu_and_menu_body(game.menu_body_ids[prow][pcol]);

#ifdef DEBUG
        sprintf(buffer, "%d %d", prow, pcol);
        mvaddnstr(SCENE_HEIGHT_ROWS, 0, buffer, MAX_MENU_OPTION_TEXT_LEN);
        sprintf(buffer, "i_focused_option %d", game.i_focused_option);
        mvaddnstr(SCENE_HEIGHT_ROWS + 1, 0, buffer, MAX_MENU_OPTION_TEXT_LEN);
        sprintf(buffer, "dlgo %d",
                game.available_menu_option_ids[game.i_focused_option]);
        mvaddnstr(SCENE_HEIGHT_ROWS + 2, 0, buffer, MAX_MENU_OPTION_TEXT_LEN);
        sprintf(buffer, "%d", game.state);
        mvaddnstr(SCENE_HEIGHT_ROWS + 3, 0, buffer, MAX_MENU_OPTION_TEXT_LEN);
        sprintf(buffer, "%u vs. %u, end %u, end-1 %d", game.player_row,
                game.last_7_rows[wrapping_sub(0, 6, game.last_7_ring_end, 1)],
                game.last_7_ring_end,
                wrapping_sub(0, 6, game.last_7_ring_end, 1));
        mvaddnstr(SCENE_HEIGHT_ROWS + 4, 0, buffer, MAX_MENU_OPTION_TEXT_LEN);
#endif
}

/* Prints the ith dialogue option */
void print_menu_option(u8 i) {
        u8 dr = MENU_BOT - 3 + i, dc = MENU_LEFT + 1;
        mvaddch(dr, dc, i + 48 + 1);
        mvaddstr(dr, dc + 1, ". ");
        mvaddnstr(dr, dc + 3,
                  menu_option_texts[game.available_menu_option_ids[i]],
                  MAX_MENU_OPTION_TEXT_LEN);
}

void draw_menu_and_menu_body(enum menu_body_id mbi) {
        u8 r, c;

        /* Top */
        mvhline(MENU_TOP, MENU_LEFT + 1, '-', MENU_WIDTH - 2);
        /* Bottom */
        mvhline(MENU_BOT, MENU_LEFT + 1, '-', MENU_WIDTH - 2);
        /* Left */
        mvvline(MENU_TOP + 1, MENU_LEFT, '|', MENU_HEIGHT - 2);
        /* Right */
        mvvline(MENU_TOP + 1, MENU_RIGHT, '|', MENU_HEIGHT - 2);

        /* Top-left corner */
        mvaddch(MENU_TOP, MENU_LEFT, '+');
        /* Top-right corner */
        mvaddch(MENU_TOP, MENU_RIGHT, '+');
        /* Bottom-left corner */
        mvaddch(MENU_BOT, MENU_LEFT, '+');
        /* Bottom-right corner */
        mvaddch(MENU_BOT, MENU_RIGHT, '+');

        /* Fill the dialogue box all black */
        for (r = MENU_TOP + 1; r < MENU_BOT; r++)
                for (c = MENU_LEFT + 1; c < MENU_RIGHT; c++)
                        mvaddch(r, c, ' ');

        /* Print current dialogue text */
        printw_wrap(menu_body_texts[mbi], MENU_TOP + 1, MENU_LEFT + 1,
                    MENU_RIGHT - 1);
}

void draw_menu(void) {
        u8 i;

        /* Draw the world first if we are not on the title screen */
        if (game.state != TITLE)
                draw_world();

        /* Draw the dialogue box on top of the world */
        draw_menu_and_menu_body(game.menu_body_id);

        /* Print either the game titles, the name of the NPC the player is
        speaking with, or the pause menu title */
        if (game.state == TITLE)
                mvaddnstr(MENU_TOP, MENU_LEFT + 4, "Elysium",
                          MAX_MENU_OPTION_TEXT_LEN);
        else if (game.state == PAUSE)
                mvaddnstr(MENU_TOP, MENU_LEFT + 4, "Menu",
                          MAX_MENU_OPTION_TEXT_LEN);
        else if (game.state == DIALOGUE)
                mvaddnstr(MENU_TOP, MENU_LEFT + 4,
                          npc_names[game.npc_ids[game.speaking_npc_row]
                                                [game.speaking_npc_col]],
                          MAX_MENU_OPTION_TEXT_LEN);

        /* Print available dialogue options */

        /* Print all the options before the focused option */
        for (i = 0; i < game.i_focused_option; i++)
                print_menu_option(i);

        /* Print the focused option */
        attron(A_STANDOUT);
        print_menu_option(game.i_focused_option);
        attroff(A_STANDOUT);

        /* Print the remaining options */
        for (i = game.i_focused_option + 1;
             i < game.n_available_menu_option_ids; i++)
                print_menu_option(i);
}

void update_game(void) {
}

void game_t_enter_state(enum game_state state) {
        /* Set the game's state to the given state */
        game.state = state;
        /* Call the appropriate state entrance function */
        switch (state) {
        case LEVEL:
                break;
        case TITLE:
        case PAUSE:
        case DIALOGUE:
                game.i_focused_option = 0;
                break;

        case N_GAME_STATES:
                break;
        }
}

/* Move the player to the new position */
void player_step_on(u8 row, u8 col) {
        game.player_row = row;
        game.player_col = col;
        if (game.npc_d_quest_active) {
                append_player_pos_to_ring();
                if (check_npc_d_quest()) {
                        game.npc_d_quest_active = 0;
                        game.menu_body_ids[npc_row(Dimitrios)]
                                          [npc_col(Dimitrios)] = D_6;
                }
        }
}

boolean handle_player_interaction(u8 row, u8 col) {
        enum entity_kind ek = game.entity_kinds[row][col];
        enum terrain_kind tk = game.terrain_kinds[row][col];
        enum menu_body_id menu_body_id = game.menu_body_ids[row][col];
        u8 entity_color = game.entity_colors[row][col],
           portal_dest_row = game.portal_dest_rows[row][col],
           portal_dest_col = game.portal_dest_cols[row][col];

        switch (ek) {
        case EMPTY:
                if (!(tk == LAVA) && (!is_water(tk) || flag(HAS_SURFBOARD))) {
                        player_step_on(row, col);
                        return TRUE;
                }
                return FALSE;
        case NPC:
                game.speaking_npc_row = row;
                game.speaking_npc_col = col;
                open_menu(menu_body_id);
                game_t_enter_state(DIALOGUE);
                return TRUE;
        case WALL:
        case ekWINDOW:
                return FALSE;
        case DOOR:
                if (game_has_key(entity_color)) {
                        /* If the player has this key, remove this door */
                        game.entity_kinds[row][col] = EMPTY;
                        player_step_on(row, col);
                        return TRUE;
                } else {
                        /* Otherwise, don't move */
                        return FALSE;
                }
        case KEY:
                /* Get the key */
                game_get_key(entity_color);
                /* Remove it from the world */
                game.entity_kinds[row][col] = EMPTY;
                /* Check which key we got. */
                /* NOTE(bpp): This is not a very smart way to check this sort of
                   thing but oh well. */
                switch (game.entity_colors[row][col]) {
                case COLOR_RED:
                        /* Flag for X's quest */
                        game.menu_body_ids[npc_row(Xavier)][npc_col(Xavier)] =
                                X_6;
                        break;
                }
                /* Step where the key was */
                player_step_on(row, col);
                return TRUE;
        case PORTAL:
                return handle_player_interaction(portal_dest_row,
                                                 portal_dest_col);
        case SIGN:
                player_step_on(row, col);
                return TRUE;
        case SURFBOARD:
                /* "Pickup" the surfboard by removing it from the tile and
                marking the player as having the surfboard */
                /* NOTE(bpp): This is brittle because assume the surfboard is on
                a grass tile */
                game.entity_kinds[row][col] = EMPTY;
                turn_on_flag(HAS_SURFBOARD);
                /* Start NPC S's quest */
                game.menu_body_ids[npc_row(Sui)][npc_col(Sui)] = S_2;
                player_step_on(row, col);
                return TRUE;
        case PURPLE_HIBISCUS:
                game.entity_kinds[row][col] = EMPTY;
                turn_on_flag(HAS_HIBISCUS);
                player_step_on(row, col);
                break;
        case SEASHELL:
                game.entity_kinds[row][col] = EMPTY;
                /* Complete NPC S's quest */
                game.menu_body_ids[npc_row(Sui)][npc_col(Sui)] = S_6;
                player_step_on(row, col);
                break;
        case TREASURE:
                game.entity_kinds[row][col] = EMPTY;
                /* Complete NPC M's quest */
                menu_vars.npc_m_quest_stage = 2;
                player_step_on(row, col);
                break;
        default:
                return TRUE;
        }
        return TRUE;
}

void draw_tile(u8 row, u8 col, u8 drow, u8 dcol) {
        enum entity_kind ek = game.entity_kinds[row][col];
        enum terrain_kind tk = game.terrain_kinds[row][col];
        enum npc_id npc_id = game.npc_ids[row][col];
        int color_pair, bold = A_BOLD * (ek == NPC);
        u8 fg = (ek == EMPTY ? game.terrain_colors[row][col] :
                               game.entity_colors[row][col]);
        char glyph;

        if (ek == EMPTY)
                glyph = terrain_glyphs[tk];
        else if (ek == NPC)
                glyph = 'A' + npc_id;
        else
                glyph = entity_glyphs[ek];

        color_pair = color_pair_index(fg, COLOR_BLACK);
        mvaddch(drow, dcol, glyph);
        mvchgat(drow, dcol, 1, bold, color_pair, NULL);
}

boolean is_water(enum terrain_kind tk) {
        return tk == WATER_OCEAN || tk == WATER_LAKE;
}

void append_player_pos_to_ring(void) {
        game.last_7_ring_end = (game.last_7_ring_end + 1) % 7;
        game.last_7_rows[game.last_7_ring_end] = game.player_row;
        game.last_7_cols[game.last_7_ring_end] = game.player_col;
}

/* Returns true if the last ith previous row and column equal row and col,
respectively. Expects 1 <= i <= 7 */
boolean compare_previous_i_row_and_column(u8 i, u8 row, u8 col) {
        u8 j = wrapping_sub(0, 6, game.last_7_ring_end, i);
        return row == game.last_7_rows[j] && col == game.last_7_cols[j];
}

boolean check_npc_d_quest(void) {
        u8 r = game.player_row;
        u8 c = game.player_col;
        u8 dr = npc_row(Dimitrios);
        u8 dc = npc_col(Dimitrios);
        u8 npc_d_ring_rows[8];
        u8 npc_d_ring_cols[8];
        int ring_i = -1;
        int i;
        int j;
        boolean next_to_d = 0;

        /* NOTE(bpp): This function assumes that NPC D is not placed on the edge
        of the game world. */

        /* N */
        npc_d_ring_rows[0] = dr - 1;
        /* NE */
        npc_d_ring_rows[1] = dr - 1;
        /* E */
        npc_d_ring_rows[2] = dr;
        /* SE */
        npc_d_ring_rows[3] = dr + 1;
        /* S */
        npc_d_ring_rows[4] = dr + 1;
        /* SW */
        npc_d_ring_rows[5] = dr + 1;
        /* W */
        npc_d_ring_rows[6] = dr;
        /* NW */
        npc_d_ring_rows[7] = dr - 1;

        /* N */
        npc_d_ring_cols[0] = dc;
        /* NE */
        npc_d_ring_cols[1] = dc + 1;
        /* E */
        npc_d_ring_cols[2] = dc + 1;
        /* SE */
        npc_d_ring_cols[3] = dc + 1;
        /* S */
        npc_d_ring_cols[4] = dc;
        /* SW */
        npc_d_ring_cols[5] = dc - 1;
        /* W */
        npc_d_ring_cols[6] = dc - 1;
        /* NW */
        npc_d_ring_cols[7] = dc - 1;

        assert(game.npc_d_quest_active && "NPC D quest is not active\n");

        /* Check that the player is next to NPC D. */
        for (i = -1; i < 2; i++) {
                for (j = -1; j < 2; j++) {
                        if (i == 0 && j == 0) {
                                continue;
                        }
                        next_to_d |= (dr + i == r) && (dc + j == c);
                }
        }
        if (!next_to_d) {
                return 0;
        }

        /* Find which position the player is in the ring around NPC D. */
        for (i = 0; i < 8; i++) {
                if (npc_d_ring_rows[i] == game.player_row &&
                    npc_d_ring_cols[i] == game.player_col) {
                        ring_i = i;
                        break;
                }
        }
        if (-1 == ring_i) {
                return 0;
        }

        /* Check that the player's last seven moves were the last seven
        positions in the ring. */
        for (i = 1; i <= 6; i++) {
                j = wrapping_sub(0, 7, ring_i, i);
                if (!compare_previous_i_row_and_column(i, npc_d_ring_rows[j],
                                                       npc_d_ring_cols[j])) {
                        return 0;
                }
        }

        return 1;
}

/* Wrapping subtraction, inclusive at both ends. */
int wrapping_sub(int min, int max, int n, int i) {
        return n - i >= min ? n - i : max + n - i + 1;
}

boolean flag(u8 flag) {
        return (game.flags >> flag) & (1);
}

void turn_on_flag(u8 flag) {
        game.flags |= (1 << flag);
}

void turn_off_flag(u8 flag) {
        game.flags &= ~(1 << flag);
}

boolean game_has_key(u8 color) {
        return (game.keys >> color) & 1;
}

void game_get_key(u8 color) {
        game.keys |= (1 << color);
}

boolean has_convinced_npc(u8 npc_id) {
        return (game.convinced_nps >> npc_id) & 1;
}

void convince_npc(u8 npc_id) {
        game.convinced_nps |= (1 << npc_id);
}

void set_speaker_menu_id(u8 menu_id) {
        game.menu_body_ids[game.speaking_npc_row][game.speaking_npc_col] =
                menu_id;
}

void convince_speaker(void) {
        convince_npc(
                game.npc_ids[game.speaking_npc_row][game.speaking_npc_col]);
}

void set_npc_row_col(u8 npc_id, u8 row, u8 col) {
        game.npc_rows[npc_id] = row;
        game.npc_cols[npc_id] = col;
}

u8 npc_row(u8 npc_id) {
        return game.npc_rows[npc_id];
}
u8 npc_col(u8 npc_id) {
        return game.npc_cols[npc_id];
}
