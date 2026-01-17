#include "draw.h"
#include "bitmap.h"
void bitmap_fill_rect(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t width,
                      uint32_t height, bool white) {
  for (int i = 2; i < width; i++) {
    for (int b = 2; b < height; b++) {
      bitmap_set_pixel(bmp, i, b, white);
    }
  }
}
void bitmap_draw_hline(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t length,
                       bool white) {
  for (int i = 0; i < length; i++) {
    bitmap_set_pixel(bmp, x, length, white);
  }
}
