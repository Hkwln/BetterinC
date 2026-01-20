#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef struct {
  uint8_t *buffer;
  size_t write_pos;
  size_t read_index;
  size_t capacity;
  size_t count; // number of bytes currently stored
} CircularBuffer;

CircularBuffer *create_buffer(size_t capacity);
void destroy_buffer(CircularBuffer *cbuf);
bool cb_write(CircularBuffer *cb, uint8_t byte);
bool cb_read(CircularBuffer *cb, uint8_t *byte);
size_t cb_write_bulk(CircularBuffer *cb, const uint8_t *data, size_t size);
size_t cb_read_bulk(CircularBuffer *cb, uint8_t *dest, size_t size);
bool cb_peek(CircularBuffer *cb, uint8_t *byte, size_t offset);
size_t cb_space_available(CircularBuffer *cb);
size_t cb_data_available(CircularBuffer *cb);
void cb_clear(CircularBuffer *cb);
void cb_write_overwrite(CircularBuffer *cb, uint8_t byte);
