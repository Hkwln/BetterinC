#pragma once
#include "data.h"
#include <stdbool.h>
#include <stdint.h>

void bitmap_fill_rect(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t width,
                      uint32_t height, bool white);
void bitmap_draw_hline(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t length,
                       bool white);
