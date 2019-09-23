#include "main.h"

signed
main (void) {

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, continuous);
    keypad(stdscr, TRUE);

    cells = (size_t )(ROWS * COLUMNS);

    bitbuffer(uint8_t, board, cells);
    uint8_t * counts = calloc(sizeof(uint8_t), cells);

    if ( !board  ) { goto cleanup; }
    if ( !counts ) { goto cleanup; }

    srand((unsigned )time(NULL));

    setup:
        run_state = 0;

    for ( size_t i = 0; i < cells; ++ i ) {
        if ( !!(rand() % 2) ) {
            setbit(board, i);
        } else { // needed for resetting the board
            unsetbit(board, i);
        }
    }

    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);

    int c = 1;
    for ( struct timespec t = { .tv_nsec = 125000000 }; (c = getch()); nanosleep(&t, 0) ) {
        switch ( c ) {
            case ' ':
                continuous = !continuous;
                nodelay(stdscr, continuous);
                c = 1;
                break;

            case KEY_RIGHT: break;

            case KEY_UP:
                t.tv_nsec -= 25000000;
                break;

            case KEY_DOWN:
                t.tv_nsec += 25000000;
                break;

            case 'q': goto cleanup;

            default:
                if ( continuous ) {
                    break;
                }
        }

        switch ( run_state ) {
            case 1: goto cleanup;
            case 2: goto setup;
            default: break;
        }

        print_board(board);
        count_neighbors(board, counts);
        evolve(board, counts);
    }

    cleanup:
        endwin();

        if ( board  ) { free(board); }
        if ( counts ) { free(counts); }

        return EXIT_SUCCESS;
}

void
count_neighbors (uint8_t * board, uint8_t * counts) {

    for ( size_t i = 0, y = 0; i < cells; ++ i, y += !(i % COLUMNS) ) {
        size_t x = i % COLUMNS;
        counts[i] = 0;

        // in-order, check liveness from upper-left to lower-right (skipping the current cell)
        if ( x && y ) { counts[i] += getbit(board, i - COLUMNS - 1); }
        if ( y ) { counts[i] += getbit(board, i - COLUMNS); }
        if ( x+1 < COLUMNS && y ) { counts[i] += getbit(board, i - COLUMNS + 1); }

        if ( x ) { counts[i] += getbit(board, i - 1); }
        if ( x+1 < COLUMNS ) { counts[i] += getbit(board, i + 1); }

        if ( x && y+1 < ROWS ) { counts[i] += getbit(board, i + COLUMNS - 1); }
        if ( y+1 < ROWS ) { counts[i] += getbit(board, i + COLUMNS); }
        if ( x+1 < COLUMNS && y+1 < ROWS ) { counts[i] += getbit(board, i + COLUMNS + 1); }
    }
}

void
evolve (uint8_t * board, uint8_t * counts) {

    for ( size_t i = 0; i < cells; ++ i ) {
        if ( counts[i] == 3 || (counts[i] == 2 && getbit(board, i)) ) {
            setbit(board, i);
        } else {
            unsetbit(board, i);
        }
    }
}

void
print_board (uint8_t * board) {

    for ( size_t i = 0, y = 0; i < cells; ++ i, y += !(i % COLUMNS) ) {
        size_t x = i % COLUMNS;
        mvprintw((signed )y, (signed )x, "%1s", getbit(board, i) ? "⬝" : " ");
    } refresh();
}

void
signal_handler (signed signum) {

    caught_signum = (sig_atomic_t )signum;
    run_state = 1 + (signum == SIGHUP);
}
