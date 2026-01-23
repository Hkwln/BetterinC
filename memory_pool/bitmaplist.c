#include "bitmaplist.h"
#include "../bitmap_image_man/src/bitmap.h"
#include <errno.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* FIXME:
 * PERFORMANCE KILLER:
 * - ändere bitmap_set_pixel zu von haus aus programmieren, oder ändere dies bei
 * der bitmap implementation
 *  - bitmap zu groß
 *  - füge bounds checking hinzu ca 62 63 freeslot = byte_idx * 8 + bit_idx
 * könnte freeslot >= capacity sein beim letzten Byte
 *  */

// info: if bit = 0 free if bit = 1 in use;
BitmapPool *bitmap_pool_create(size_t object_size, size_t capacity) {
  BitmapPool *bmp = malloc(sizeof(BitmapPool));
  if (bmp == NULL) {
    fprintf(stderr, "%s", strerror(errno));
    perror(NULL);
    return NULL;
  }
  bmp->object_size = object_size;
  bmp->capacity = capacity;
  bmp->size = 0;
  errno = 0;
  bmp->memory = calloc(capacity, object_size);
  if (bmp->memory == NULL) {
    fprintf(stderr, "%s", strerror(errno));
    perror(NULL);
    return NULL;
  }
  bmp->bm = bitmap_create(
      (capacity),
      1); // Wird als uint32_t interpretiert also ist 0 oke sonst 1
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
  if (pool->capacity > pool->size) {
    // check where the next free slot is
    size_t freeslot = SIZE_MAX;
#if 0
        //SLOW non ffs variant
    for (size_t i = 0; i < pool->capacity; i++) {
      if (!bitmap_get_pixel(pool->bm, i, 0)) {
        freeslot = i;
        break;
      }
    }
#endif
#if 1
    // Optimized: Scan bytes, use CPU instruction
    for (size_t byte_idx = 0; byte_idx < (pool->capacity + 7) / 8; byte_idx++) {
      uint8_t byte = pool->bm->data[byte_idx];
      if (byte != 0xFF) { // Found a free bit (assuming 1=used)
        int bit_idx = __builtin_ffs(~byte) - 1; // Find first 0-bit
        freeslot = byte_idx * 8 + bit_idx;
        break;
      }
    }
#endif
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
    fprintf(stderr, "the pool size is to smal to fit this object, free one "
                    "object before you "
                    "add this\n");
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
        fprintf(stderr,
                "something in the defragmantion did not quite work right");
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
