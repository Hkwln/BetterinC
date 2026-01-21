#include "bitmaplist.h"
#include "../bitmap_image_man/src/bitmap.h"
#include <stdlib.h>
BitmapPool *bitmap_pool_create(size_t object_size, size_t capacity) {
  BitmapPool *bmp = malloc(sizeof(BitmapPool));
  bmp->object_size = object_size;
  bmp->capacity = capacity;
  bmp->current = 0;
  bmp->memory = calloc(capacity, object_size);
  // XXX: ist es nicht überflüssig, object size oder generelle eine height in
  // meiner bitmap zu speichern? ich will ja einglich nur wissen ob sie schon
  // benutz(1) oder noch nicht benutz(0) ist oder muss ich die capacity als
  // height geben?
  bmp->bm = bitmap_create(capacity, 0);
  return bmp;
}
void bitmap_pool_destroy(BitmapPool *bmp) {
  bmp->object_size = 0;
  bmp->capacity = 0;
  bmp->current = 0;
  free(bmp->memory);
  bitmap_destroy(bmp->bm);
  free(bmp);
}
// TODO:
void *bitmap_pool_alloc(BitmapPool *pool) {
  if (pool->memory == NULL)
    return NULL;
  // XXX: meine vorgehensweise: check ob current pos gerade 0 oder 1 ist, wenn
  // null dann ist der eintrag noch leer und ich kann meinen eintrag machen;
  // dann geh pos+1 und mach das ganze nochmal
  // FIXME: pool->bm->data müsste falsch sein, oder?
  void *result = pool->bm->data;
  bitmap_set_pixel(pool->bm, pool->current, 0, 1);
  pool->current++;
  return result;
}
void bitmap_pool_free(BitmapPool *pool, void *ptr) {

  bitmap_get_pixel(pool->bm, pool->current - 1, 0);
  bitmap_set_pixel(pool->bm, pool->current - 1, 0, 0);
}
