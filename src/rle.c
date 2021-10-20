#include "rle.h"

struct bounding_box
smallest_bounding_box (uint8_t * board, size_t rows, size_t columns) {

    struct bounding_box box = { .north = rows, .west = columns };
    size_t cells = rows * columns;

    for ( size_t i = 0, y = 0, x = 0; i < cells; ++ i, y += !(i % columns), x = i % columns ) {
        if ( getbit(board, i) ) {
            if ( y < box.north ) { box.north = y; }
            if ( y > box.south ) { box.south = y; }
            if ( x < box.west  ) { box.west  = x; }
            if ( x > box.east  ) { box.east  = x; }
        }
    }

    return box;
}

void
put_rle (FILE * outf, uint8_t * board, size_t rows, size_t columns) {

    struct bounding_box box = smallest_bounding_box(board, rows, columns);
    size_t bound_columns = box.east + 1 - box.west;
    size_t bound_rows = box.south + 1 - box.north;

    fprintf(outf, "x = %zu, y = %zu\n", bound_columns, bound_rows);

    size_t northwest = box.north * columns + box.west;
    size_t southeast = box.south * columns + box.east;

    for ( size_t i = northwest, count = 1; i <= southeast; i += count, count = 1) {
        size_t x = i % columns;
        if ( x < box.west || x > box.east ) { continue; }

        bool cur = getbit(board, i);

        for ( size_t j = 1; x + j <= box.east; ++ j ) {
            if ( getbit(board, j + i) != cur ) {
                break;
            }

            ++count;
        }

        if ( !cur && x + count - 1 == box.east) {
            fputc('$', outf);
            continue;
        }

        if ( count > 1 ) { fprintf(outf, "%zu", count); }
        fputc(cur ? 'o' : 'b', outf);

        if ( x + count - 1 == box.east && (i + count - 1) != southeast ) {
            fputc('$', outf);
        }
    }

    fputc('!', outf);
}

bool
get_rle (FILE * inf, uint8_t ** board, size_t * rows, size_t * columns) {

    fscanf(inf, "x = %zu, y = %zu\n", columns, rows);

    size_t cells = *rows * *columns;
    *board = init_bitbuffer(uint8_t, cells);

    size_t index = 0;
    while (index <= cells) {
        size_t count = 0;
        int br = fscanf(inf, "%zu", &count);
        if ( br < 1 ) { count = 1; }

        char tag [2] = { 0 };
        fscanf(inf, "%1[!$ob]", tag);

        if ( index == cells ) {
            return *tag == '!';
        }

        switch ( *tag ) {
            case '!':
                return (index + 1) == cells;

            case '$':
                if ( (index % *columns) ) {
                    index += *columns - (index % *columns);
                } continue;

            case 'o':
                for ( size_t i = 0; i < count; ++ i ) {
                    setbit(*board, index + i);
                }
                __attribute__((fallthrough));
            case 'b':
                index += count;
                continue;

            default:
                continue;
        }
    }

    return false;
}

