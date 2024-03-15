#include "game.h"
#include "types.h"
#include <ncurses.h>

struct game_t game;

int main(void) {
        /* Input character */
        int ch;
        /* Whether we should to update and redraw the game */
        boolean should_to_update_and_draw;
        /* Set up the game */
        init_game();
        /* Draw the game once before starting the game loop */
        draw_game();
        /* Main loop */
        do {
                /* Get the next input character */
                ch = getch();
                /* Handle input */
                should_to_update_and_draw = handle_input(ch);
                if (should_to_update_and_draw) {
                        /* Update the game */
                        update_game();
                        /* Draw the game */
                        draw_game();
                }
                /* Quit the game */
        } while (!flag(QUIT));
        /* Tear down the game */
        end_game();
        return 0;
}
