#pragma once
#include <stdbool.h>
#include <stdio.h>

bool render_to_svg(const char *lstring, const char *filename,
                   size_t iterations);
