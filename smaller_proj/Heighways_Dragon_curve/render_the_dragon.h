#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Streams the order-n_generations dragon curve straight into an SVG file
// using O(1) memory: segments are generated on the fly, no L-system string
// is ever built.  n_generations must be in [1, 62].
bool render_to_svg(size_t n_generations, const char *filename);
