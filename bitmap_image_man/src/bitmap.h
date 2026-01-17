#pragma once
#include "data.h"
#include <stdbool.h>
Bitmap *bitmap_create(uint32_t width, uint32_t height);
void bitmap_destroy(Bitmap *bmp);
void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white);
bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y);
void bitmap_save_ascii(Bitmap *bmp, const char *filename);
void bitmap_save_pbm(Bitmap *bmp, const char *filename);
