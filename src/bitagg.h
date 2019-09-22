#if !defined(BIT_AGGREGATE_H)
#define BIT_AGGREGATE_H

#pragma once

#include <limits.h>

#define bitsof(x) (sizeof(x) * CHAR_BIT)
#define members_needed(type, length) (((length) + bitsof(type) - 1) / bitsof(type))

#define bitarray(type, name, length) type name [(((length) + bitsof(type) - 1) / bitsof(type))] = { 0 }
#define bitbuffer(type, name, length) type * name = calloc((((length) + bitsof(type) - 1) / bitsof(type)), sizeof(type))

#define getbit(arr, idx)   (!!((arr)[(idx) / bitsof(*(arr))] &   (1 << ((idx) % bitsof(*(arr))))))
#define setbit(arr, idx)      ((arr)[(idx) / bitsof(*(arr))] |=  (1 << ((idx) % bitsof(*(arr)))))
#define unsetbit(arr, idx)    ((arr)[(idx) / bitsof(*(arr))] &= ~(1 << ((idx) % bitsof(*(arr)))))

#endif // BIT_AGGREGATE_H

