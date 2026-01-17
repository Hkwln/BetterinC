/*here happens all the magic :)*/
#include "bitmap.h"
#include "data.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// TODO: check if this is correct
Bitmap *bitmap_create(uint32_t width, uint32_t height) {
  // allocate memory to bitmap
  Bitmap *bitmap = malloc(sizeof(int));
  bitmap = calloc(3, bytes_needed(width, height));
  return bitmap;
}
// FIXME: check for memory leaks
void bitmap_destroy(Bitmap *bmp) {
  bmp->height = 0;
  bmp->width = 0;
  bmp->data = NULL;
  free(bmp);
}

/*TODO: finish function*/
void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = byte_index % 8;
  byte_index = bit_index ^ (0 << bit_offset);
}

/* TODO: */
bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = byte_index % 8;
  // now check the if the pixel is toggled or not and return the value
  return byte_index & 1;
}
