// aim is to implement a morton code x1y1x2y2 usw
#include <stdint.h>
#include <stdio.h>
// time 30min
uint32_t morton_decode(uint16_t x, uint16_t y) {
  uint32_t both = 0;
  for (int i = 0; i < 32 * 8; i++) {
    both |= ((x << i) & 1) >> (i * 2) | (((y << i)) & 1) >> ((2 * i) + 1);
  }
  return both;
}

int main() {
  uint32_t morton = morton_decode(17, 111);
  printf(" both:%u \n", morton); // 3
  return 0;
}

// 17 = 0_0_1_0_0_0_1_
// 111 = 1_1_0_1_1_1_1
// insgesamt: 01011001010111  = 5719
