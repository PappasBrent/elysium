#ifndef ELYSIUM_PRINTW_WRAP_H
#define ELYSIUM_PRINTW_WRAP_H

/* Moves to position y,x and print the string s. Wraps at column w. Expects that
all whitespace characters are spaces. */
extern void printw_wrap(const char *s, int y, int dx, long unsigned int w);

#endif
