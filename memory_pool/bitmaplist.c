#include "bitmaplist.h"
#include "../bitmap_image_man/src/bitmap.h"
#include <linux/limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
bool debug = 1;
// info: if bit = 0 free if bit = 1 in use;
//  FIXME: missing NULL check bei malloc und calloc
BitmapPool *bitmap_pool_create(size_t object_size, size_t capacity) {
  BitmapPool *bmp = malloc(sizeof(BitmapPool));
  if (bmp == NULL) {
    return NULL;
  }
  bmp->object_size = object_size;
  bmp->capacity = capacity;
  bmp->size = 0;
  bmp->memory = calloc(capacity, object_size);
  bmp->bm = bitmap_create(
      capacity, 0); // Wird als uint32_t interpretiert also ist 0 oke sonst 1
  // init poolsats:
  bmp->poolstats.total_allocations = 0;
  bmp->poolstats.total_frees = 0;
  bmp->poolstats.peak_usage = 0;
  return bmp;
}
void bitmap_pool_destroy(BitmapPool *bmp) {
  free(bmp->memory);
  bitmap_destroy(bmp->bm);
  free(bmp);
}
void *bitmap_pool_alloc(BitmapPool *pool) {
  if (pool->memory == NULL)
    return NULL;
  if (pool->capacity > pool->size) {
    // check where the next free slot is
    size_t freeslot = SIZE_MAX;
    // TODO: optimize to use ffs  scan bytes, use cpu instructions
    for (size_t i = 0; i < pool->capacity; i++) {
      if (!bitmap_get_pixel(pool->bm, i, 0)) {
        freeslot = i;
        break;
      }
    }
    if (freeslot == SIZE_MAX)
      return NULL;
    void *result = (char *)pool->memory + (freeslot * pool->object_size);
    bitmap_set_pixel(pool->bm, freeslot, 0, 1);
    pool->size++;
    pool->poolstats.total_allocations++;
    if (pool->size > pool->poolstats.peak_usage)
      pool->poolstats.peak_usage = pool->size;
    return result;
  } else {
#if debug
    printf("the pool is to smal to fit this object, free one object before you "
           "add this\n");
#endif
    return NULL;
  }
  return NULL;
}
void bitmap_pool_free(BitmapPool *pool, void *ptr) {
  size_t offset = (char *)ptr - (char *)pool->memory;
  size_t index = offset / pool->object_size;
  if (offset % pool->object_size != 0)
    return;
  if (index >= pool->capacity)
    return;

  bitmap_set_pixel(pool->bm, index, 0, 0);
  pool->size--;
  pool->poolstats.total_frees++;
}
void bitmap_pool_defragment(BitmapPool *pool) {

  // scan for 0 in between 1 in the bitmap
  for (int i = 0; i < pool->capacity; i++) {
    if (!bitmap_get_pixel(pool->bm, i, 0)) {
      // if size> currentposition search from right to left the next 1 and move
      // the memory into the currentposition;
      if (pool->size > i) {
        for (int j = pool->capacity; j > i; j--) {
          if (bitmap_get_pixel(pool->bm, j, 0)) {
            // shift the memory from the old bit to pos i
            // 1. safe the old memory in a temp memory
            // 2. copy the old memory into the new position
            memcpy(pool->memory + (i * pool->object_size),
                   pool->memory + (j * pool->object_size), pool->object_size);
            // 3. free old memory with bitmap_pool_free
            bitmap_pool_free(pool, pool->memory + (j * pool->object_size));
          }
        }
      } else {
        break;
      }
    }
  }
}

void bool_print_stats(BitmapPool *pool) {
  printf("Größe der bitmap in bytes %zu\n",
         (pool->capacity * pool->object_size));
  printf("total frees:%zu \t total_allocations:%zu \n",
         pool->poolstats.total_frees, pool->poolstats.total_allocations);
  printf("currently in use: %zu \n", pool->size);
  printf("peak usage: %zu", pool->poolstats.peak_usage);
}
