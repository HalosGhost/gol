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

#define primbitop(arr, idx, op) ((arr)[(idx) / bitsof(*(arr))] op (1 << ((idx) % bitsof(*(arr)))))

#define getbit(arr, idx)    (!!primbitop((arr), (idx), &   ))
#define setbit(arr, idx)       primbitop((arr), (idx), |=  )
#define unsetbit(arr, idx)     primbitop((arr), (idx), &= ~)
#define togglebit(arr, idx)    primbitop((arr), (idx), ^=  )

#define assignbit(arr, idx, val) \
    do { \
        if ( val ) { \
            setbit((arr), (idx)); \
        } else { \
            unsetbit((arr), (idx)); \
        } \
    } while (0)

#endif // BIT_AGGREGATE_H

