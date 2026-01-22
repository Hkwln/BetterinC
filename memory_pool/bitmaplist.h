#pragma once
#include "../bitmap_image_man/src/data.h"
#include "shared_struct.h"
#include <stdio.h>
typedef struct {
  void *memory;
  Bitmap *bm;
  size_t object_size;
  size_t capacity;
  size_t size;
  PoolStats poolstats;
} BitmapPool;
BitmapPool *bitmap_pool_create(size_t object_size, size_t capacity);
void bitmap_pool_destroy(BitmapPool *bmp);
void *bitmap_pool_alloc(BitmapPool *pool);
void bitmap_pool_free(BitmapPool *pool, void *ptr);
void bitmap_pool_defragment(BitmapPool *pool);
void bbool_print_stats(BitmapPool *pool);
