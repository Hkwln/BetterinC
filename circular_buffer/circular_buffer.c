#include "circular_buffer.h"
#include <stdlib.h>

CircularBuffer *create_buffer(size_t capacity) {
  CircularBuffer *cbuf;
  cbuf->capacity = capacity;
  // XXX: i am not sure with this:
  cbuf->buffer = calloc(sizeof(capacity), 0);
  cbuf->count = 0;
  cbuf->read_index = 0;
  cbuf->write_pos = 0;
  return cbuf;
}
void destroy_buffer(CircularBuffer *cbuf) {
  cbuf->count = -1;
  cbuf->read_index = -1;
  cbuf->write_pos = -1;
  free(cbuf->buffer);
}
bool cb_read(CircularBuffer *cb, uint8_t *byte) {
  if (cb->count < 0) {
    return 1;
  } else {

    return 0;
  }
}

// read single byte:
bool cb_write(CircularBuffer *cb, uint8_t *byte) {
  // check if buffer is empty
  if (cb->count < 0) {
    // write byte to the buffer:
    cb->buffer[cb->write_pos] = *byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->count++;
    return 1;
  } else {
    printf("error your circular buffer is empty or not initialized");
    return 0;
  }
}
