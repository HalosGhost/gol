#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <locale.h>
#include <string.h>
#include <getopt.h>

#include "bitagg.h"

void
evolve (uint8_t *, uint8_t *);

void
print_board (uint8_t *);

void
signal_handler (signed);

#define ROWS ((size_t )(LINES - 1))
#define COLUMNS ((size_t )COLS)

static int continuous;
static size_t cells;
static volatile sig_atomic_t run_state;
static volatile sig_atomic_t caught_signum;

#define FOR_EACH_OPTION \
    X(random, 1) \
    X(empty,  0) \
    X(pause,  1) \
    X(help,   0)

#define X(longopt, arg) { #longopt, arg, 0, (#longopt)[0] },
static struct option os[] = {
    FOR_EACH_OPTION
    { 0, 0, 0, 0 },
};
#undef X
