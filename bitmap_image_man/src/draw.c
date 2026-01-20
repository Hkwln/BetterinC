#include "draw.h"
#include "bitmap.h"
// x and y are the starting point width and height are the size of the rectangle
void bitmap_fill_rect(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t width,
                      uint32_t height, bool white) {
  for (int i = x; i < x + width; i++) {
    for (int b = y; b < y + height; b++) {
      bitmap_set_pixel(bmp, i, b, white);
    }
  }
}
void bitmap_draw_hline(Bitmap *bmp, uint32_t x, uint32_t y, uint32_t length,
                       bool white) {
  for (int i = x; i < x + length; i++) {
    bitmap_set_pixel(bmp, i, y, white);
  }
}
