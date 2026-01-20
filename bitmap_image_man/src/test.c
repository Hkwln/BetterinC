// here everything works together:
#include "bitmap.h"
#include "data.h"
#include "draw.h"

int main(int argc, char **argv) {
  Bitmap *bitmap = bitmap_create(32, 32);
  bitmap_fill_rect(bitmap, 2, 2, 28, 28, 1);
  // draw a cross i ist der startpunkt des kreuzes
  for (int i = 8; i < (bitmap->height - 8); i++) {
    if (i == 16) {
      bitmap_draw_hline(bitmap, 8, i, 16, 0);
    } else {
      bitmap_draw_hline(bitmap, 16, i, 1, 0);
    }
  }
  bitmap_save_ascii(bitmap, "testfile.txt");

  bitmap_destroy(bitmap);
  return 0;
}
