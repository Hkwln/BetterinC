/*here happens all the magic :)*/
#include "bitmap.h"
#include "data.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// TODO: check if this is correct
Bitmap *bitmap_create(uint32_t width, uint32_t height) {
  // allocate memory to bitmap
  Bitmap *bitmap = malloc(sizeof(int));
  bitmap = calloc(3, bytes_needed(width, height));
  return bitmap;
}
// FIXME: check for memory leaks
void bitmap_destroy(Bitmap *bmp) { free(bmp); }

/*TODO: finish function*/
void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white);

/* TODO: */
bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y);
