#include "main.h"

signed
main (void) {

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    cur  = (uint8_t ** )malloc(sizeof(uint8_t *) * ROWS);
    next = (uint8_t ** )malloc(sizeof(uint8_t *) * ROWS);

    for ( size_t i = 0; i < ROWS; ++ i ) {
        cur[i]  = (uint8_t * )malloc(sizeof(uint8_t *) * COLUMNS);
        next[i] = (uint8_t * )malloc(sizeof(uint8_t *) * COLUMNS);
    }

    srand((unsigned )time(NULL));

    setup:
        run_state = 0;

    for ( size_t i = 0; i < ROWS; ++ i ) {
        for ( size_t j = 0; j < COLUMNS; ++ j ) {
            cur[i][j] = !!(rand() % 2);
        }
    }

    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);

    for ( struct timespec t = { .tv_nsec = 125000000 }; ; nanosleep(&t, 0) ) {
        switch ( run_state ) {
            case 1: goto cleanup;
            case 2: goto setup;
            default: break;
        }

        print_board(cur);
        evolve(cur, next);
    }


    cleanup:
        endwin();
        fputs("Caught interrupt; exiting\n", stderr);
        for ( size_t i = 0; i < ROWS; ++ i ) {
            if ( cur[i]  ) { free(cur[i]);  }
            if ( next[i] ) { free(next[i]); }
        }

        if ( cur  ) { free(cur); }
        if ( next ) { free(next); }

        exit(EXIT_SUCCESS);
}

uint8_t
neighbor_count (size_t x, size_t y, uint8_t ** b) {

    uint8_t total = 0;

    if ( x && y ) { total += b[x-1][y-1]; } // top-left
    if ( x ) { total += b[x-1][y]; } // top-center
    if ( x && y+1 < COLUMNS ) { total += b[x-1][y+1]; } // top-right

    if ( y ) { total += b[x][y-1]; } // middle-left
    if ( y+1 < COLUMNS ) { total += b[x][y+1]; } // middle-right

    if ( y && x+1 < ROWS ) { total += b[x+1][y-1]; } // bottom-left
    if ( x+1 < ROWS ) { total += b[x+1][y]; } // bottom-center
    if ( x+1 < ROWS && y+1 < COLUMNS ) { total += b[x+1][y+1]; } // bottom-right

    return total;
}

void
evolve (uint8_t ** o, uint8_t ** n) {

    for ( size_t i = 0; i < ROWS; ++ i ) {
        for ( size_t j = 0; j < COLUMNS; ++ j ) {
            uint8_t nc = neighbor_count(i, j, o);
            n[i][j] = nc == 3 || (nc == 2 && o[i][j]);
        }
    }

    for ( size_t i = 0; i < ROWS; ++ i ) {
        for ( size_t j = 0; j < COLUMNS; ++ j ) {
            o[i][j] = n[i][j];
        }
    }
}

void
print_board (uint8_t ** board) {

    for ( size_t i = 0; i < ROWS; ++ i ) {
        for ( size_t j = 0; j < COLUMNS; ++ j ) {
            mvprintw((signed )i, (signed )j, "%s", board[i][j] ? "⬝" : " ");
        }
    } refresh();
}

void
signal_handler (signed signum) {

    caught_signum = (sig_atomic_t )signum;
    run_state = 1 + (signum == SIGHUP);
}
