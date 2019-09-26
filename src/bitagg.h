#if !defined(BIT_AGGREGATE_H)
#define BIT_AGGREGATE_H

#pragma once

#include <limits.h>

#define bitsof(x) (sizeof(x) * CHAR_BIT)
#define members_needed(type, length) (((length) + bitsof(type) - 1) / bitsof(type))

#define bitarray(type, name, length) type name [members_needed(type, length)] = { 0 }
#define bitbuffer(type, name, length) type * name = calloc(members_needed(type, length), sizeof(type))

/**
 * Note: All macros below this block are unhygenic
 **
 * Because all of these macros evaluate at least one argument twice, passing
 * them any expression that causes a side-effect is unsafe.
 */

#define getbit(arr, idx)   (!!((arr)[(idx) / bitsof(*(arr))] &   (1 << ((idx) % bitsof(*(arr))))))

#define setbit(arr, idx)      ((arr)[(idx) / bitsof(*(arr))] |=  (1 << ((idx) % bitsof(*(arr)))))
#define unsetbit(arr, idx)    ((arr)[(idx) / bitsof(*(arr))] &= ~(1 << ((idx) % bitsof(*(arr)))))
#define togglebit(arr, idx)   ((arr)[(idx) / bitsof(*(arr))] ^=  (1 << ((idx) % bitsof(*(arr)))))

#define assignbit(arr, idx, val) \
    do { \
        if ( val ) { \
            setbit((arr), (idx)); \
        } else { \
            unsetbit((arr), (idx)); \
        } \
    } while (0)

#endif // BIT_AGGREGATE_H

