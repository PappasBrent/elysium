#include "menu_manual.h"
#include "constants.h"
#include "entity.h"
#include "game.h"
#include "menu.h"
#include "ncurses.h"
#include "stdlib.h"
#include "tile.h"
#include "time.h"
#include "types.h"
#include "world.h"

static const char *__restrict__ save_file_name = ".elysium.save";

boolean file_exists(const char *s);

cmd save_game, load_game_title, load_game_pause;

boolean file_exists(const char *s) {
        FILE *fp = fopen(s, "r");
        boolean exists = fp != NULL;
        if (exists)
                fclose(fp);
        return exists;
}

void save_game(void) {
        FILE *fp = fopen(save_file_name, "wb");
        /* Save the game */
        if (fp) {
                if (1 != fwrite(&game, sizeof(struct game_t), 1, fp))
                        open_menu(SAVE_FAIL);
                else
                        open_menu(SAVE_SUCCESSFUL);
                fclose(fp);
        } else
                open_menu(SAVE_FAIL);
}

void load_game_title(void) {
        FILE *fp = fopen(save_file_name, "rb");
        boolean success = (fp != NULL);
        if (success)
                success = (1 == fread(&game, sizeof(struct game_t), 1, fp));
        if (success) {
                game_t_enter_state(LEVEL);
                fclose(fp);
        } else
                open_menu(TITLE_LOAD_FAIL);
}

void load_game_pause(void) {
        FILE *fp = fopen(save_file_name, "rb");
        boolean success = (fp != NULL);
        if (success)
                success = (1 == fread(&game, sizeof(struct game_t), 1, fp));
        if (success) {
                game_t_enter_state(LEVEL);
                fclose(fp);
        } else
                open_menu(PAUSE_LOAD_FAIL);
}

void open_title_menu(void) {
        if (file_exists(save_file_name))
                open_menu(TITLE_LOAD);
        else
                open_menu(TITLE_NO_LOAD);
}

void open_pause_menu(void) {
        if (file_exists(save_file_name))
                open_menu(PAUSE_LOAD);
        else
                open_menu(PAUSE_NO_LOAD);
}

void try_load_game_title(void) {
        if (file_exists(save_file_name))
                load_game_title();
        else
                open_menu(TITLE_LOAD_FAIL);
}

void try_load_game_pause(void) {
        if (file_exists(save_file_name))
                load_game_pause();
        else
                open_menu(PAUSE_LOAD_FAIL);
}

static const u8 flower_colors[3] = { COLOR_CYAN, COLOR_RED, COLOR_YELLOW };

void new_game(void) {
        u8 r, c, n_flowers = 0, i;
        u8 flower_positions[SCENE_HEIGHT_ROWS * SCENE_WIDTH_COLS][2] = { 0 };

        srand(time((void *)0));
        /* Load the world's tiles */
        for (r = 0; r < WORLD_HEIGHT_ROWS; r++)
                for (c = 0; c < WORLD_WIDTH_COLS; c++) {
                        switch (base_world[r][c]) {
                        case '@':
/* Place the tutorial sign under the player */
#ifndef DEBUG
                                game.entity_kinds[r][c] = SIGN;
                                game.entity_colors[r][c] = COLOR_WHITE;
                                game.menu_body_ids[r][c] = TUTORIAL_SIGN;
#else
                                game.entity_kinds[r][c] = EMPTY;
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
#endif

                                game.player_row = r;
                                game.player_col = c;
                                break;
                        case ',':
                                game.terrain_kinds[r][c] = GRASS;
                                game.terrain_colors[r][c] = COLOR_GREEN;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '+':
                                game.terrain_kinds[r][c] = GRASS;
                                game.terrain_colors[r][c] =
                                        flower_colors[rand() % 3];
                                game.entity_kinds[r][c] = EMPTY;
                                flower_positions[n_flowers][0] = r;
                                flower_positions[n_flowers][1] = c;
                                n_flowers++;
                                break;
                        case '.':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '-':
                                game.terrain_kinds[r][c] = WATER_LAKE;
                                game.terrain_colors[r][c] = COLOR_BLUE;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '~':
                                game.terrain_kinds[r][c] = WATER_OCEAN;
                                game.terrain_colors[r][c] = COLOR_CYAN;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '^':
                                game.terrain_kinds[r][c] = SAND;
                                game.terrain_colors[r][c] = COLOR_YELLOW;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '*':
                                game.terrain_kinds[r][c] = LAVA;
                                game.terrain_colors[r][c] = COLOR_RED;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '#':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = WALL;
                                break;
                        case ':':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = ekWINDOW;
                                break;
                        case '/':
                                game.terrain_kinds[r][c] = SAND;
                                game.terrain_colors[r][c] = COLOR_YELLOW;
                                game.entity_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = SURFBOARD;
                                break;
                        case '>':
                                game.terrain_kinds[r][c] = WATER_OCEAN;
                                game.terrain_colors[r][c] = COLOR_CYAN;
                                game.entity_colors[r][c] = COLOR_RED;
                                game.entity_kinds[r][c] = SEASHELL;
                                break;
                        case '$':
                                game.terrain_kinds[r][c] = SAND;
                                game.terrain_colors[r][c] = COLOR_YELLOW;
                                game.entity_colors[r][c] = COLOR_GREEN;
                                game.entity_kinds[r][c] = TREASURE;
                                break;
                        case '1':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_BLACK;
                                game.entity_kinds[r][c] = DOOR;
                                game.entity_colors[r][c] = COLOR_RED;
                                break;
                        case 'b':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_BLACK;
                                game.entity_kinds[r][c] = KEY;
                                game.entity_colors[r][c] = COLOR_RED;
                                break;
                        case '5':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = DOOR;
                                game.entity_colors[r][c] = COLOR_MAGENTA;
                                break;
                        case 'e':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = KEY;
                                game.entity_colors[r][c] = COLOR_MAGENTA;
                                break;
#define NPC_ID(ID, LETTER, NEW_GAME_DIALOGUE_ID, ENDING_DIALOGUE_ID, \
               TERRAIN_KIND, TERRAIN_COLOR, COLOR)                   \
        case LETTER:                                                 \
                game.npc_ids[r][c] = ID;                             \
                game.menu_body_ids[r][c] = NEW_GAME_DIALOGUE_ID;     \
                game.terrain_kinds[r][c] = TERRAIN_KIND;             \
                game.terrain_colors[r][c] = TERRAIN_COLOR;           \
                game.entity_colors[r][c] = COLOR;                    \
                game.entity_kinds[r][c] = NPC;                       \
                set_npc_row_col(ID, r, c);                           \
                break;
                                NPC_IDS
#undef NPC_ID
                        }
                }

        /* Randomly replace a flower with the purple hibiscus */
        i = rand() % n_flowers;
        r = flower_positions[i][0];
        c = flower_positions[i][1];
        game.terrain_colors[r][c] = COLOR_GREEN;
        game.entity_kinds[r][c] = PURPLE_HIBISCUS;
        game.entity_colors[r][c] = COLOR_MAGENTA;

        /* Enter the world */
        game_t_enter_state(LEVEL);
}

void load_ending_level(void) {
        u8 r, c;
        /* Load the world's tiles */
        for (r = 0; r < SCENE_HEIGHT_ROWS; r++)
                for (c = 0; c < SCENE_WIDTH_COLS; c++) {
                        switch (ending_world[r][c]) {
                        case '@':
                                game.entity_kinds[r][c] = EMPTY;
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.player_row = r;
                                game.player_col = c;
                                break;
                        case ',':
                                game.terrain_kinds[r][c] = GRASS;
                                game.terrain_colors[r][c] = COLOR_GREEN;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '+':
                                game.terrain_kinds[r][c] = GRASS;
                                game.terrain_colors[r][c] =
                                        flower_colors[rand() % 3];
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '.':
                                game.terrain_kinds[r][c] = STONE;
                                game.terrain_colors[r][c] = COLOR_WHITE;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '-':
                                game.terrain_kinds[r][c] = WATER_LAKE;
                                game.terrain_colors[r][c] = COLOR_BLUE;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '~':
                                game.terrain_kinds[r][c] = WATER_OCEAN;
                                game.terrain_colors[r][c] = COLOR_CYAN;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '^':
                                game.terrain_kinds[r][c] = SAND;
                                game.terrain_colors[r][c] = COLOR_YELLOW;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
                        case '*':
                                game.terrain_kinds[r][c] = LAVA;
                                game.terrain_colors[r][c] = COLOR_RED;
                                game.entity_kinds[r][c] = EMPTY;
                                break;
#define NPC_ID(ID, LETTER, NEW_GAME_DIALOGUE_ID, ENDING_DIALOGUE_ID,   \
               TERRAIN_KIND, TERRAIN_COLOR, COLOR)                     \
        case LETTER:                                                   \
                if (has_convinced_npc(ID)) {                           \
                        game.npc_ids[r][c] = ID;                       \
                        game.menu_body_ids[r][c] = ENDING_DIALOGUE_ID; \
                        game.terrain_kinds[r][c] = STONE;              \
                        game.terrain_colors[r][c] = COLOR_WHITE;       \
                        game.entity_colors[r][c] = COLOR;              \
                        game.entity_kinds[r][c] = NPC;                 \
                } else {                                               \
                        game.terrain_kinds[r][c] = STONE;              \
                        game.terrain_colors[r][c] = COLOR_WHITE;       \
                        game.entity_kinds[r][c] = EMPTY;               \
                }                                                      \
                break;
                                NPC_IDS
#undef NPC_ID
                        }
                }
}

void no_overwrite(void) {
        return_to_pause_menu();
}

void quit_game(void) {
        turn_on_flag(QUIT);
}

void pause_game(void) {
        open_pause_menu();
        game_t_enter_state(PAUSE);
}

void enter_state_level(void) {
        game_t_enter_state(LEVEL);
}

void return_to_title_menu(void) {
        open_title_menu();
}

void return_to_pause_menu(void) {
        open_pause_menu();
}

void try_save_game(void) {
        if (file_exists(save_file_name))
                open_menu(OVERWRITE_PROMPT);
        else
                save_game();
}

void yes_overwrite(void) {
        save_game();
}

void guess_npc_o_riddle(void) {
        boolean l, i, v, e;
        u8 dr = MENU_BOT - 3, dc = MENU_LEFT + 1;

        open_menu(GUESS_NPC_O_RIDDLE);
        draw_game();
        mvaddnstr(dr, dc, "(Now type a four-letter word)",
                  MAX_MENU_OPTION_TEXT_LEN);
        l = getch() == 'l';
        i = getch() == 'i';
        v = getch() == 'v';
        e = getch() == 'e';

        if (l && i && v && e) {
                set_speaker_menu_id(O_2);
                open_menu(O_2);
        } else {
                open_menu(O_3);
        }
}
