#if !defined(BIT_AGGREGATE_H)
#define BIT_AGGREGATE_H

#pragma once

#include <limits.h>
#include <stdlib.h>

#define bitsof(x) (sizeof(x) * CHAR_BIT)
#define members_needed(type, length) (((length) + bitsof(type) - 1) / bitsof(type))

#define decl_bitbuffer(type, name) type * name
#define init_bitbuffer(type, length) calloc(members_needed(type, (length)), sizeof(type))

#define bitbuffer(type, name, length) decl_bitbuffer(type, name) = init_bitbuffer(type, length)

/**
 * Note: All macros below this block are unhygenic
 **
 * Because all of these macros evaluate at least one argument twice, passing
 * them any expression that causes a side-effect is unsafe.
 */

#define primbitop(arr, idx, op) ((arr)[(idx) / bitsof(*(arr))] op (1 << ((idx) % bitsof(*(arr)))))

#define getbit(arr, idx)    (!!primbitop((arr), (idx), &   ))
#define setbit(arr, idx)       primbitop((arr), (idx), |=  )
#define clearbit(arr, idx)     primbitop((arr), (idx), &= ~)
#define togglebit(arr, idx)    primbitop((arr), (idx), ^=  )

#define assignbit(arr, idx, val) \
    ((val) ? setbit((arr), (idx)) : clearbit((arr), (idx)))

#endif // BIT_AGGREGATE_H

