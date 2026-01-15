#pragma once
#include <stdint.h>

typedef struct {

  uint32_t width;
  uint32_t height;
  uint8_t *data;
} Bitmap;
int bytes_needed(uint32_t width, uint32_t height);
