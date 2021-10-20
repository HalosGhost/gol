#if !defined(RLE_H)
#define RLE_H

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include <inttypes.h>

#include "bitagg.h"

struct bounding_box {
    size_t north, south, east, west;
};

struct bounding_box
smallest_bounding_box (uint8_t *, size_t, size_t);

void
put_rle (FILE *, uint8_t *, size_t, size_t);

bool
get_rle (FILE *, uint8_t **, size_t *, size_t *);

#endif
