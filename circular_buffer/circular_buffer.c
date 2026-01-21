#include "circular_buffer.h"
#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

CircularBuffer *create_buffer(size_t capacity) {
  CircularBuffer *cbuf;
  cbuf = malloc(sizeof(CircularBuffer));
  cbuf->capacity = capacity;
  cbuf->buffer = calloc(capacity, sizeof(uint8_t));
  cbuf->count = 0;
  cbuf->read_index = 0;
  cbuf->write_pos = 0;
  return cbuf;
}
void destroy_buffer(CircularBuffer *cbuf) {
  free(cbuf->buffer);
  free(cbuf);
}
bool cb_read(CircularBuffer *cb, uint8_t *byte) {
  if (cb->count > 0) {
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
  if (cb->count < cb->capacity) {
    // write byte to the buffer:
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->count++;
    return 1;
  } else {
    return 0;
  }
}
// write multiple bytes:
size_t cb_write_bulk(CircularBuffer *cb, const uint8_t *data, size_t size) {
  size_t space = cb->capacity - cb->count;
  // limit write:
  size_t write_limit = (size > space) ? space : size;
  size_t first_chunk = cb->capacity - cb->write_pos;
  if (write_limit <= first_chunk) {
    // No wrap: single memcpy
    memcpy(cb->buffer + cb->write_pos, data, write_limit);
  } else {
    // Wrap: two memcpy calls
    memcpy(cb->buffer + cb->write_pos, data, first_chunk);
    memcpy(cb->buffer, data + first_chunk, write_limit - first_chunk);
  }
  // update pos and count
  cb->write_pos = (cb->write_pos + write_limit) % cb->capacity;
  cb->count += write_limit - cb->capacity;
  return write_limit;
}
size_t cb_read_bulk(CircularBuffer *cb, uint8_t *dest, size_t size) {
  size_t space = cb->capacity - cb->count;
  size_t write_limit = (space < size) ? space : size;
  size_t first_chunk = cb->capacity - cb->read_index;
  if (write_limit <= first_chunk) {
    memcpy(cb->buffer + cb->read_index, dest, write_limit);
  } else {
    memcpy(cb->buffer + cb->read_index, dest, first_chunk);
    memcpy(cb->buffer + cb->read_index, dest, write_limit - first_chunk);
  }
  return write_limit;
}
// read without rmeoving:
bool cb_peek(CircularBuffer *cb, uint8_t *byte, size_t offset) {
  size_t help = (cb->read_index + offset) % cb->capacity;
  *byte = cb->buffer[help];
  return !(offset >= cb->count);
}
size_t cb_space_available(CircularBuffer *cb) {
  return (size_t)(cb->capacity - cb->count);
}
size_t cb_data_available(CircularBuffer *cb) { return (size_t)(cb->count); }
// clear the position but not the conten!
void cb_clear(CircularBuffer *cb) {
  cb->read_index = cb->write_pos = cb->count = 0;
}
/* TODO: newest data should overwrite oldest data when full*/
void cb_write_overwrite(CircularBuffer *cb, uint8_t byte) {
  // if not full:
  if (cb->count < cb->capacity) {
    // write byte to the buffer:
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->count++;
  } else {
    // find the oldest data because of the cbuf is full, the oldest pos is the
    // current write_pos:
    cb->buffer[cb->write_pos] = byte;
    cb->write_pos = (cb->write_pos + 1) % cb->capacity;
    cb->read_index = (cb->read_index + 1) % cb->capacity;
  }
}
