#include "printw_wrap.h"
#include "ncurses.h"
#include <stdlib.h>

void printw_wrap(const char *s, int y, int x, long unsigned int w) {
        /* Current column */
        int dx = x;
        /* Start of most recent word */
        const char *t = NULL;

        for (; *s; s++) {
                if (*s == ' ') {
                        /* End of current word */
                        if (t != NULL) {
                                if (dx + ((s - t) / sizeof(*s)) > w) {
                                        /* Need to wrap */
                                        y++;
                                        dx = x;
                                        /* Then print the word */
                                        for (; t != s; t++, dx++)
                                                mvaddch(y, dx, *t);

                                        /* Clear the current word */
                                        t = NULL;
                                } else {
                                        /* Don't wrap */
                                        for (; t != s; t++, dx++)
                                                mvaddch(y, dx, *t);
                                        t = NULL;
                                }
                        }
                        /* Print the trailing space */
                        mvaddch(y, dx++, *s);
                } else if (*s != ' ' && t == NULL) {
                        /* Start of a new word */
                        t = s;
                }
        }

        /* Print the last word */
        if (t != NULL) {
                if (dx + ((s - t) / sizeof(*s)) > w) {
                        /* Need to wrap */
                        y++;
                        dx = x;
                        /* Then print the word */
                        for (; t != s; t++, dx++)
                                mvaddch(y, dx, *t);

                        /* Clear the current word */
                        t = NULL;
                } else {
                        /* Don't wrap */
                        for (; t != s; t++, dx++)
                                mvaddch(y, dx, *t);
                        t = NULL;
                }
        }
}

