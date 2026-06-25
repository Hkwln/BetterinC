// aim is to implement a morton code x1y1x2y2 usw.
#include "morton_code.h"
#include <stdio.h>

uint32_t morton_encode(uint16_t x, uint16_t y) {
  uint32_t both = 0;
  for (int i = 0; i < 16; i++) {
    both |= ((x >> i) & 1) << (i * 2) | (((y >> i)) & 1) << ((2 * i) + 1);
  }
  return both;
}

coordinates_t morton_decode(uint32_t both) {
  coordinates_t sep;
  sep.x = 0;
  sep.y = 0;
  int power;
  for (int i = 0; i < 32; i++) {
    power = 1 << i;
    if ((both & power) != 0) {
      // set x or y to 1;
      if (i % 2 == 0) {
        sep.x |= (power >> i / 2);
      } else {
        sep.y |= power >> ((i / 2) + 1);
      }
    }
  }
  return sep;
}

int main(void) {
  uint32_t morton = morton_encode(17, 111);
  printf("both: %u \n", morton); // 10667
  coordinates_t sep = morton_decode(morton);
  printf("x: %u \n", sep.x); // 17
  printf("y: %u \n", sep.y); // 111
  return 0;
}

// 17 = 0_0_1_0_0_0_1_
// 111 = 1_1_0_1_1_1_1
// insgesamt: 01011001010111  = 10667;
