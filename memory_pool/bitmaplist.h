#pragma once
#include "../bitmap_image_man/src/data.h"
#include <stdio.h>
// XXX: should i just take my bitmap i  already have ?
typedef struct {
  void *memory;
  Bitmap *bm;
  size_t object_size;
  size_t capacity;
  size_t current;

} BitmapPool;
BitmapPool *bitmap_pool_create(size_t object_size, size_t capacity);
void bitmap_pool_destroy(BitmapPool *bmp);
void *bitmap_pool_alloc(BitmapPool *pool);
void bitmap_pool_free(BitmapPool *pool, void *ptr);
