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
