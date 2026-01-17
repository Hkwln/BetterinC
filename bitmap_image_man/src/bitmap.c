#include "bitmap.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

/* TODO:
 * - ADD BOUNDS CHECKING
 * - ADD NULL CHECK AFTER MALLOC/CALLOC*/
Bitmap *bitmap_create(uint32_t width, uint32_t height) {
  // allocate memory to bitmap
  Bitmap *bitmap = malloc(sizeof(Bitmap));
  bitmap->data = calloc(3, bytes_needed(width, height));
  return bitmap;
}
void bitmap_destroy(Bitmap *bmp) {
  bmp->height = 0;
  bmp->width = 0;
  bmp->data = NULL;
  free(bmp->data);
  free(bmp);
}

void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  bmp->data[byte_index] |= (1 << bit_offset);
}

bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  return (bmp->data[byte_index] >> bit_offset) & 1;
}
/* TODO:*/
void bitmap_save_ascii(Bitmap *bmp, const char *filename);
void bitmap_save_pbm(Bitmap *bmp, const char *filename);
