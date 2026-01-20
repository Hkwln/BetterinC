#include "bitmap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* TODO:
 * - ADD BOUNDS CHECKING
 * - ADD NULL CHECK AFTER MALLOC/CALLOC*/
Bitmap *bitmap_create(uint32_t width, uint32_t height) {
  // allocate memory to bitmap
  Bitmap *bitmap = malloc(sizeof(Bitmap));
  bitmap->data = calloc(bytes_needed(width, height), 1);
  bitmap->width = width;
  bitmap->height = height;
  return bitmap;
}
void bitmap_destroy(Bitmap *bmp) {
  free(bmp->data);
  free(bmp);
  bmp->height = 0;
  bmp->width = 0;
  bmp->data = NULL;
}

void bitmap_set_pixel(Bitmap *bmp, uint32_t x, uint32_t y, bool white) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  if (white) {
    bmp->data[byte_index] |= (1 << bit_offset);
  } else {
    bmp->data[byte_index] &= ~(1 << bit_offset);
  }
}

bool bitmap_get_pixel(Bitmap *bmp, uint32_t x, uint32_t y) {
  uint32_t bit_index = y * bmp->width + x;
  uint16_t byte_index = bit_index / 8;
  int16_t bit_offset = bit_index % 8;
  return (bmp->data[byte_index] >> bit_offset) & 1;
}
// test if this works
void bitmap_save_ascii(Bitmap *bmp, const char *filename) {
  FILE *savedfile;
  savedfile = fopen(filename, "w");
  for (int h = 0; h < bmp->height; h++) {
    for (size_t w = 0; w < bmp->width; w++) {
      bool byte = bitmap_get_pixel(bmp, w, h);
      if (byte) {
        fprintf(savedfile, "#");
      } else {
        fprintf(savedfile, ".");
      }
    }
    fprintf(savedfile, "\n");
  }
  fclose(savedfile);
}
/* TODO:*/
void bitmap_save_pbm(Bitmap *bmp, const char *filename) {
  int bitsize = 0;
  FILE *fb = fopen(filename, "wb");
  bitsize = bmp->height * bmp->width;
}
