#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef struct {
  uint32_t width;
  uint32_t height;
  uint8_t *data;
} Bitmap;
typedef struct {
  uint32_t *indices;  // flat array of active cell indices
  uint32_t count;     // number of active cells
  uint8_t *is_active; // bitmap for O(1) lookup
} Active;

int bytes_needed(uint32_t width, uint32_t height);
Active *initactive(int maxsize);
void destroyactive(Active *active);
void set_Aactive(Active *active, Bitmap *live, int dx[], int dy[]);
void set_1active(Active *active, Bitmap *live, uint32_t x, uint32_t y, int dx[],
                 int dy[]);
bool is_active(Active *active, Bitmap *live, uint32_t x, uint32_t y);
void reset_active(Active *active, Bitmap *live);
