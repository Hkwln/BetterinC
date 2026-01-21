#include "circular_buffer.h"
#include <stdint.h>
#include <stdlib.h>
int main() {
  // basic write/read
  CircularBuffer *cbuf = create_buffer(10);
  uint8_t *test;
  test = malloc(sizeof(uint8_t));
  *test = 100;
  cb_write(cbuf, *test);
  bool right = cb_read(cbuf, test);
  if (right)
    printf("write and read does work!\n");
  // fill and empty
  for (int i = 1; i < 2 + cbuf->capacity; i++) {
    uint8_t test2 = 10 + i;
    cb_write(cbuf, test2);
    printf(" size available: %zu \n", cb_space_available(cbuf));
    cb_write_overwrite(cbuf, test2);
  }
  // wrap arount

  // Bulk operations
  // peek
  // overwrite mode
  free(test);
  destroy_buffer(cbuf);
  return 0;
}
