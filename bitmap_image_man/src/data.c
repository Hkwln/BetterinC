#include "data.h"
#include "bitmap.h"
#include <stdint.h>
#include <stdlib.h>
// TODO: rework needed
int bytes_needed(uint32_t width, uint32_t height) {
  int bytes = (width * height + 7) / 8;
  return bytes;
}
Active *initactive(int maxsize) {
  Active *active = malloc(sizeof(Active));
  active->count = 0;
  active->indices = calloc(maxsize, sizeof(uint32_t));
  active->is_active = calloc(maxsize, sizeof(uint8_t));
  return active;
}
void destroyactive(Active *active) {
  active->count = -1;
  free(active->indices);
  free(active->is_active);
  free(active);
}
void set_Aactive(Active *active, Bitmap *live, int dx[], int dy[]) {
  active->count = 0;
  for (int y = 0; y < live->height; y++) {
    for (int x = 0; x < live->width; x++) {
      active->is_active[y * live->width + x] = 0;
    }
  }

  for (int y = 0; y < live->height; y++) {
    for (int x = 0; x < live->width; x++) {
      if (bitmap_get_pixel(live, x, y)) {
        uint32_t idx = y * live->width + x;
        if (!active->is_active[idx]) {
          active->indices[active->count] = idx;
          active->is_active[idx] = 1;
          active->count++;
        }

        for (int c = 0; c < 8; c++) {
          int nx = x + dx[c];
          int ny = y + dy[c];
          if (nx >= 0 && nx < live->width && ny >= 0 && ny < live->height) {
            uint32_t nidx = ny * live->width + nx;
            if (!active->is_active[nidx]) {
              active->indices[active->count] = nidx;
              active->is_active[nidx] = 1;
              active->count++;
            }
          }
        }
      }
    }
  }
}
void set_1active(Active *active, Bitmap *live, uint32_t x, uint32_t y, int dx[],
                 int dy[]) {
  uint32_t idx = y * live->width + x;
  if (!active->is_active[idx]) {
    active->indices[active->count] = idx;
    active->is_active[idx] = 1;
    active->count++;
  }

  for (int c = 0; c < 8; c++) {
    int nx = x + dx[c];
    int ny = y + dy[c];
    if (nx >= 0 && nx < live->width && ny >= 0 && ny < live->height) {
      uint32_t nidx = ny * live->width + nx;
      if (!active->is_active[nidx]) {
        active->indices[active->count] = nidx;
        active->is_active[nidx] = 1;
        active->count++;
      }
    }
  }
}
bool is_active(Active *active, Bitmap *live, uint32_t x, uint32_t y) {
  if (active->count >= 0 && active->is_active[y * live->width + x]) {
    return 1;
  } else {
    return 0;
  }
}
void reset_active(Active *active, Bitmap *live) {
  active->count = 0;
  for (int i = 0; i < live->width * live->height; i++) {
    active->is_active[i] = 0;
  }
}
void progress_bar(int max_epochs, int e) {
  int bar_width = 30;
  float progress = (float)(e + 1) / max_epochs;
  int filled = (int)(progress * bar_width);
  printf("[");
  for (int i = 0; i < bar_width; i++) {
    if (i < filled)
      printf("=");
    else if (i == filled)
      printf(">");
    else
      printf(" ");
  }
  printf("] %.1f%%\n", progress * 100);
}
