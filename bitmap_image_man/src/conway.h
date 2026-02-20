/* Conway's Game of Life - Core Logic
 * Extracted from live.c for use in Plymouth plugin
 */

#pragma once

#include "bitmap.h"
#include "data.h"
#include <stdint.h>
#include <stdlib.h>

/* Check how many neighbors a cell has */
static inline int getnachbarn(Bitmap *live, uint32_t x, uint32_t y, int dx[],
                              int dy[]) {
  int nachbarn = 0;
  for (int c = 0; c < 8; c++) {
    int nx = x + dx[c];
    int ny = y + dy[c];
    if (nx >= 0 && nx < live->width && ny >= 0 && ny < live->height &&
        bitmap_get_pixel(live, nx, ny))
      nachbarn++;
  }
  return nachbarn;
}

/* Spawn new pixels - randomly or near active areas */
static inline void spawn(size_t count, Bitmap *live, Active *active, int dx[],
                         int dy[]) {
  for (int i = 0; i < count; i++) {
    int x, y;
    if (active->count > 0 && rand() % 100 < 90) {
      int random_idx = rand() % active->count;
      int idx = active->indices[random_idx];
      x = idx % live->width;
      y = idx / live->width;
    } else {
      x = rand() % (live->width);
      y = rand() % (live->height);
      bitmap_set_pixel(live, x, y, 1);
      set_Aactive(active, live, dx, dy);
    }
  }
}

/* Conway's Game of Life update - one generation */
static inline int loop(Bitmap *live, Bitmap *live2, Active *active,
                       Active *next_active, int dx[], int dy[]) {
  int changes = 0;

  /* Clear destination bitmap */
  for (int y = 0; y < live2->height; y++) {
    for (int x = 0; x < live2->width; x++) {
      bitmap_set_pixel(live2, x, y, 0);
    }
  }

  reset_active(next_active, live2);

  /* Apply Conway's rules to active cells */
  for (int i = 0; i < active->count; i++) {
    int x = active->indices[i] % live->width;
    int y = active->indices[i] / live->width;
    bool pixel = bitmap_get_pixel(live, x, y);
    int nach = getnachbarn(live, x, y, dx, dy);

    if (pixel == 1 && (nach == 2 || nach == 3)) {
      bitmap_set_pixel(live2, x, y, 1);
      set_1active(next_active, live2, x, y, dx, dy);
    } else if (pixel == 0 && nach == 3) {
      bitmap_set_pixel(live2, x, y, 1);
      set_1active(next_active, live2, x, y, dx, dy);
      changes++;
    } else if (pixel == 1) {
      changes++;
    }
  }

  return changes;
}
