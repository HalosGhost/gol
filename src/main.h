#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <locale.h>

uint8_t
neighbor_count (size_t, size_t, uint8_t **);

void
evolve (uint8_t **, uint8_t **);

void
print_board (uint8_t **);

void
signal_handler (signed);

static uint8_t ** cur, ** next;
static volatile sig_atomic_t run_state;
static volatile sig_atomic_t caught_signum;

#define ROWS ((size_t )LINES)
#define COLUMNS ((size_t )COLS)
