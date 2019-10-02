#include "main.h"

signed
main (signed argc, char * argv[]) {

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED, NULL);

    cells = (size_t )(ROWS * COLUMNS);

    bitbuffer(uint8_t, back, cells);
    bitbuffer(uint8_t, forth, cells);

    if ( !back   ) { goto cleanup; }
    if ( !forth  ) { goto cleanup; }

    srand((unsigned )time(NULL));

    uint8_t rate = 0;
    uint8_t pause = 125;
    uint8_t help = 0;
    for ( signed oi = 0, c = getopt_long(argc, argv, "r:p:eh", os, &oi);
         c != -1;
                         c = getopt_long(argc, argv, "r:p:eh", os, &oi)) {

        switch ( c ) {
            case 'e':
                rate = 0;
                continuous = FALSE;
                break;

            case 'r':
                sscanf(optarg, "%" SCNu8, &rate);
                rate %= 101;
                continuous = TRUE;
                break;

            case 'p':
                sscanf(optarg, "%" SCNu8, &pause);
                break;

            case 'h':
                help = 1;
                goto cleanup;
        }
    }

    struct timespec t = { .tv_nsec = pause * 1000000 };
    setup:
        run_state = 0;

    for ( size_t i = 0; i < cells; ++ i ) {
        assignbit(back, i, rand() % 100 < rate);
    }

    nodelay(stdscr, continuous);

    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);

    MEVENT ev = { 0 };
    size_t gen = 0;
    int c = 1;
    uint8_t * board = back;

    mainloop:
    do {
        int evenGen = !(gen % 2);
        board = evenGen ? back : forth;
        print_board(board);
        attron(A_REVERSE);
        mvprintw(ROWS, 0, "generation %zu | evolving every ", gen);
        if ( !continuous ) {
            printw("∞");
        } else {
            printw("%ld", t.tv_nsec / 1000000);
        }
        printw(" ms | random density: %" PRIu8"/100\n", rate);
        attroff(A_REVERSE);

        c = getch();

        switch ( c ) {
            case ' ':
                continuous = !continuous;
                nodelay(stdscr, continuous);
                break;

            case KEY_RIGHT: break;

            case KEY_UP:
                t.tv_nsec -= 25000000;
                if ( t.tv_nsec < 0 ) { t.tv_nsec = 0; }
                continue;

            case KEY_DOWN:
                t.tv_nsec += 25000000;
                if ( t.tv_nsec > 250000000 ) { t.tv_nsec = 250000000; }
                continue;

            case KEY_NPAGE:
                rate -= 5;
                if ( rate > 100 ) { rate = 100; }
                continue;

            case KEY_PPAGE:
                rate += 5;
                if ( rate > 100 ) { rate = 100; }
                continue;

            case 'q': goto cleanup;
            case 'r': goto setup;

            case KEY_MOUSE:
                if ( continuous ) { break; }
                getmouse(&ev);
                togglebit(board, ev.y * COLUMNS + ev.x);
                goto mainloop;

            default: if ( continuous ) { break; }
        }

        switch ( run_state ) {
            case 1: goto cleanup;
            case 2: goto setup;
            default: break;
        }

        nanosleep(&t, 0);
        ++ gen;

        if ( evenGen ) {
            evolve(back, forth);
        } else {
            evolve(forth, back);
        }
    } while ( c );

    cleanup:
        endwin();

        if ( help ) {
            for ( struct option * o = os; o && o->name; ++ o ) {
                printf("%c, %s %s\n", o->val, o->name, o->has_arg ? "ARG" : "");
            }
        }

        if ( back  ) { free(back);  }
        if ( forth ) { free(forth); }

        return EXIT_SUCCESS;
}

void
evolve (uint8_t * current, uint8_t * next) {

    for ( size_t i = 0, y = 0, x = 0; i < cells; ++ i, y += !(i % COLUMNS), x = i % COLUMNS ) {
        uint8_t count = 0;

        if ( x && y ) { count += getbit(current, i - COLUMNS - 1); }
        if ( y ) { count += getbit(current, i - COLUMNS); }
        if ( x+1 < COLUMNS && y ) { count += getbit(current, i - COLUMNS + 1); }

        if ( x ) { count += getbit(current, i - 1); }
        if ( x+1 < COLUMNS ) { count += getbit(current, i + 1); }

        if ( x && y+1 < ROWS ) { count += getbit(current, i + COLUMNS - 1); }
        if ( y+1 < ROWS ) { count += getbit(current, i + COLUMNS); }
        if ( x+1 < COLUMNS && y+1 < ROWS ) { count += getbit(current, i + COLUMNS + 1); }

        assignbit(next, i, count == 3 || (count == 2 && getbit(current, i)));
    }
}

void
print_board (uint8_t * board) {

    for ( size_t i = 0, y = 0; i < cells; ++ i, y += !(i % COLUMNS) ) {
        size_t x = i % COLUMNS;
        mvprintw((signed )y, (signed )x, "%1s", getbit(board, i) ? "⬝" : " ");
    }
}

void
signal_handler (signed signum) {

    caught_signum = (sig_atomic_t )signum;
    run_state = 1 + (signum == SIGHUP);
}
