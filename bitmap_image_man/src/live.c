// aim is to create the game of live with the help of the bitmap :)
/*RULES:
 * A live cell with fewer than two live neighbors dies (underpopulation).
 * A live cell with two or three live neighbors survives.
 * A live cell with more than three live neighbors dies (overpopulation).
 * A dead cell with exactly three live neighbors becomes alive (reproduction).
 */
#include "bitmap.h"
#include "data.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
typedef struct {
  unsigned int x;
  unsigned int y;
  bool spawn;
} should_spawn;
// Zählen Diagonale pixel auch als nachbarn?
int nachbarn(Bitmap *live, uint32_t i, uint32_t n) {
  // TODO: nutze schleife mit offset array anstatt so viele iff schleifen:
  // nt dx[] = {-1,0,1,-1,1,-1,0,1}; int dy[] = {-1,-1,-1,0,0,1,1,1};
  int nachbarn = 0;
  if (i + 1 < live->height && bitmap_get_pixel(live, i + 1, n))
    nachbarn++;
  if (i > 0 && bitmap_get_pixel(live, i - 1, n))
    nachbarn++;
  if (n + 1 < live->width && bitmap_get_pixel(live, i, n + 1))
    nachbarn++;
  if (n > 0 && bitmap_get_pixel(live, i, n - 1))
    nachbarn++;
  if (n > 0 && i + 1 < live->height && bitmap_get_pixel(live, i + 1, n - 1))
    nachbarn++;
  if (n + 1 < live->width && i + 1 < live->height &&
      bitmap_get_pixel(live, i + 1, n + 1))
    nachbarn++;
  if (n > 0 && i > 0 && bitmap_get_pixel(live, i - 1, n - 1))
    nachbarn++;
  if (n + 1 < live->width && i > 0 && bitmap_get_pixel(live, i - 1, n + 1))
    nachbarn++;

  return nachbarn;
}

void print_bitmap(Bitmap *bmp) {
  for (int h = 0; h < bmp->height; h++) {
    for (int w = 0; w < bmp->width; w++) {
      bool pixel = bitmap_get_pixel(bmp, w, h);
      printf("%c", pixel ? '#' : '.');
    }
    printf("\n");
  }
}
// XXX: vllt lasse den user die göße und die schnelligkeit bestimmen?
int main(int argc, char **argv) {
  Bitmap *live = bitmap_create(32, 32);
  srand(time(NULL));
  int ninitial_pixel = rand() % (live->height << 4);
  for (int i = 0; i < ninitial_pixel; i++) {
    int randomx = (rand() % live->height - 2) + 2;
    int randomy = (rand() % live->width - 2) + 2;
    bitmap_set_pixel(live, randomx, randomy, 1);
  }
  size_t epoch = 500;
  should_spawn spawn[live->width * live->height];

  system("clear");
  printf("Generation 0:\n");
  print_bitmap(live);
  usleep(50000);

  for (int e = 0; e < epoch; e++) {
    int b = 0;
    for (int i = 0; i < live->height; i++) {
      for (int n = 0; n < live->width; n++) {
        bool pixel = bitmap_get_pixel(live, i, n);
        int nach = nachbarn(live, i, n);
        if (pixel == 1 && (nach < 2 || nach > 3)) {
          spawn[b].x = i;
          spawn[b].y = n;
          spawn[b].spawn = 0;
          b++;
        } else if (nach == 3 && pixel == 0) {
          spawn[b].x = i;
          spawn[b].y = n;
          spawn[b].spawn = 1;
          b++;
        }
      }
    }
    // setting pixels
    if (b == 0) {
      exit(0);
    }
    for (int t = 0; t < b; t++) {
      bitmap_set_pixel(live, spawn[t].x, spawn[t].y, spawn[t].spawn);
    }

    system("clear");
    printf("Generation %d:\n", e + 1);
    print_bitmap(live);
    usleep(50000);
  }
  bitmap_destroy(live);
  return 0;
}
