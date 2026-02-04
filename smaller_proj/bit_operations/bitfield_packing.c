#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// TIME: 30 min
// TODO: schau dir noch RGB565 an

/* Packs the uint64 memory in a uint8_t portions and returns them
 * returns an array of 8 uint8_t if it works
 * returns -1 if fails*/
uint8_t *pack(uint64_t bit) {
  int size = sizeof(bit) / sizeof(uint8_t);
  // XXX: WO Free()?
  uint8_t *packet_bits = malloc(size * sizeof(uint8_t));
  for (int i = 0; i < size; i++) {
    printf("%d,  ", i);
    // Take the first 8 bits and store them in ;
    packet_bits[i] = (uint8_t)(bit >> 8 * i);
  }
  printf("\n");
  // FIXME: fix this mess
  if (packet_bits == 0) {
    return (uint8_t *)-1;
  } else {
    return packet_bits;
  }
}
int main() {
  uint64_t bit = 0x1234;
  printf("%d \n", pack(bit)[0]);
  return 0;
}
