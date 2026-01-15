#include <stdint.h>

int bytes_needed(uint32_t width, uint32_t height) {
  int bytes = (width * height + 7) / 8;
  return bytes;
}
