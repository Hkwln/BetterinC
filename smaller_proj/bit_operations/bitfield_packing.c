#include <stdint.h>
#include <stdio.h>
// TIME: 30 min +27 min + 11min +  20min = 1h 28 min
struct rgb888 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
uint16_t rgb888_to_rgb565(struct rgb888 bit) {
  uint16_t rgb565 = 0;
  uint8_t r = bit.r >> 3;
  uint8_t g = bit.g >> 2;
  uint8_t b = bit.b >> 3;
  rgb565 = (r << 11);
  rgb565 = rgb565 | (g << 5);
  rgb565 = rgb565 | b;
  return rgb565;
}
struct rgb888 rgb565_to_rgb888(uint16_t rgb565) {
  struct rgb888 rgb;
  rgb.r = (rgb565 >> 11) << 3;
  rgb.g = (~rgb565 >> 11) & (rgb565 >> 5) << 2;
  rgb.b = (~rgb565 >> 5) & (rgb565) << 3;
  return rgb;
}
int main() {
  struct rgb888 rgb;
  rgb.r = 10;
  rgb.g = 20;
  rgb.b = 30;
  uint16_t rgb565 = rgb888_to_rgb565(rgb);
  printf("%d\n", rgb565);
  struct rgb888 rgb2 = rgb565_to_rgb888(rgb565);
  return 0;
}
