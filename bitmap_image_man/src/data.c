#include "data.h"
#include "bitmap.h"
#include <stdint.h>
#include <stdlib.h>
int bytes_needed(uint32_t width, uint32_t height) {
  int bytes = (width * height + 7) / 8;
  return bytes;
}
Active *initactive(int maxsize) {
  Active *active = malloc(sizeof(Active));
  active->count = 0;
  active->indices = calloc(maxsize / 8, 1);
  active->is_active = 0;
  return active;
}
void destroyactive(Active *active) {
  active->count = -1;
  free(active->indices);
}
// layout of the cells are: 11 12 13 14 15... 21 22 23 24
// is active = 0000 0000 0000 0000
// 0000
// 0000
// 0000
// 0001 zb für pos x = 4 y = 4 in the cell
//  sets this pixel and neigbours to activ
void set_active(Active *active, Bitmap *live, uint32_t x, uint32_t y) {
  // check if it is already active:
  uint gridx = x / 8;
  uint gridy = x / 8;
  uint cell = gridx + live->width / 8 + gridy + live->height / 8;
  // is this grid currently active?
  if (active->count >= 0 && active->indices[cell] != 0) {
    return;
  }
  active->is_active; // =pos of x and y in the grid
  if (active->is_active == on the edge) {
    if (active->indices[cell] != on the edge) {
      active->indices[cell + 1] = 1;
      active->indices[cell - 1] = 1;
      active->indices[cell + live->width / 8] = 1;
      active->indices[cell - live->width / 8] = 1;
    }
  }
  active->indices[cell] = 1;
  active->count++;
}
