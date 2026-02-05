#include <stdint.h>
#include <stdio.h>
// TIME: 30 min +27 min
// TODO: schau dir noch RGB565 an
struct rgb888 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
/* Packs the uint64 memory in a uint8_t portions and returns them
 * returns an array of 8 uint8_t if it works
 * returns -1 if fails*/
uint16_t rgb888_to_rgb565(struct rgb888 bit) {
  // XXX: WO Free()?
  uint16_t rgb565 = 0;
  // first we do red:
  uint8_t r = bit.r >> 3;
  printf(" %d", r);
  // Green:
  uint8_t g = bit.g >> 2;
  uint8_t b = bit.b >> 3;
  return rgb565;
}
int main() {
  struct rgb888 rgb;
  rgb.r = 10;
  rgb.g = 20;
  rgb.b = 30;
  uint16_t rgb565 = rgb888_to_rgb565(rgb);

  printf("%d\n", rgb565);
  // expect 2212
  return 0;
}
