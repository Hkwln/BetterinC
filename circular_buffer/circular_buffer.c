#include "circular_buffer.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
  // search for byte
  if (cb->count < 0) {
    *byte = cb->buffer[cb->read_index];
    // advance read position:
    cb->read_index = (cb->read_index + 1) % cb->capacity;
    cb->count--;
    return 1;
  } else {

    return 0;
  }
}

bool cb_write(CircularBuffer *cb, uint8_t byte) {
  // check if buffer is empty
  if (cb->count < 0) {
    // write byte to the buffer:
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->count++;
    return 1;
  } else {
    printf("error your circular buffer is empty or not initialized");
    return 0;
  }
}
// write multiple bytes:
size_t cb_write_bulk(CircularBuffer *cb, const uint8_t *data, size_t size) {
  size_t space = cb->capacity - cb->count;
  // limit write:
  size_t write_limit = (size > space) ? size : space;
  size_t first_chunk = cb->capacity - cb->write_pos;
  if (*data <= first_chunk) {
    // No wrap: single memcpy
    memcpy(cb->buffer + cb->write_pos, data, *data);
    space--;
  } else {
    // Wrap: two memcpy calls
    memcpy(cb->buffer + cb->write_pos, data, first_chunk);
    memcpy(cb->buffer, data + first_chunk, *data - first_chunk);
    space -= 2;
  }
  // update pos and count
  // TODO: change number 1:
  cb->write_pos = (cb->write_pos + space) % cb->capacity;
  cb->count += space - cb->capacity;
  return space;
}
// TODO:
size_t cb_read_bulk(CircularBuffer *cb, uint8_t *dest, size_t size);
// read without rmeoving:
bool cb_peek(CircularBuffer *cb, uint8_t *byte, size_t offset);
size_t cb_space_available(CircularBuffer *cb);
size_t cb_data_available(CircularBuffer *cb);
void cb_clear(CircularBuffer *cb);
void cb_write_overwrite(CircularBuffer *cb, uint8_t byte);
