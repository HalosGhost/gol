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

#include "bitagg.h"

void
count_neighbors (uint8_t *, uint8_t *);

void
evolve (uint8_t *, uint8_t *);

void
print_board (uint8_t *);

void
signal_handler (signed);

#define ROWS ((size_t )LINES)
#define COLUMNS ((size_t )COLS)

static int continuous = TRUE;
static size_t cells;
static volatile sig_atomic_t run_state;
static volatile sig_atomic_t caught_signum;

