// aim is to create the game of live with the help of the bitmap :)
/*RULES:
 * A live cell with fewer than two live neighbors dies (underpopulation).
 * A live cell with two or three live neighbors survives.
 * A live cell with more than three live neighbors dies (overpopulation).
 * A dead cell with exactly three live neighbors becomes alive (reproduction).
 */
// TODO:ziel: switche langsam zu ly von der mitte aus nach außen, game of live
// oberfläche wird quasi immer kleiner , also in tty, nicht im code,
// performance improvements: only track living cells with a type
#include "bitmap.h"
#include "data.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
/* Spawns new pixels in the bitmap*/
void spawn(size_t count, Bitmap *live) {
  for (int i = 0; i < count; i++) {
    int randomx = (rand() % live->height - 2) + 2;
    int randomy = (rand() % live->width - 2) + 2;
    bitmap_set_pixel(live, randomx, randomy, 1);
  }
}
/*Checks and returnshow many neighbors the current pixel has;*/
int getnachbarn(Bitmap *live, uint32_t i, uint32_t n, int dx[], int dy[]) {
  int nachbarn = 0;
  for (int c = 0; c < 8; c++) {
    if (dx[c] + i < live->height && dx[c] + i >= 0 && dy[c] + n < live->width &&
        dy[c] + n >= 0 && bitmap_get_pixel(live, i + dx[c], n + dy[c]))
      nachbarn++;
  }
  return nachbarn;
}
// checks for repeating pattern  returns true if it repeates
bool isrepeating(Bitmap *live, Bitmap *live2) {
  for (int i = 0; i < live->width; i++) {
    for (int n = 0; n < live->height; n++) {
      if (bitmap_get_pixel(live, i, n) != bitmap_get_pixel(live2, i, n))
        return 0;
    }
  }
  return 1;
}
// conway game of live pixel updating rule
//  b = changes
void loop(Bitmap *live, Bitmap *live2, size_t *b, int dx[], int dy[]) {
  for (int i = 0; i < live->height; i++) {
    for (int n = 0; n < live->width; n++) {
      bool pixel = bitmap_get_pixel(live, i, n);
      // set_active(active, randomx, randomy, dx, dy);
      int nach = getnachbarn(live, i, n, dx, dy);
      if (pixel == 1 && (nach < 2 || nach > 3)) {
        bitmap_set_pixel(live2, i, n, 0);
        (*b)++;
      } else if (nach == 3 && pixel == 0) {
        bitmap_set_pixel(live2, i, n, 1);
        (*b)++;
      } else if (pixel == 1 && (nach == 2 || nach == 3)) {
        bitmap_set_pixel(live2, i, n, 1);
      } else {
        bitmap_set_pixel(live2, i, n, 0);
      }
    }
  }
}
// XXX: vllt lasse den user die göße und die schnelligkeit bestimmen?
int main(int argc, char **argv) {
  uint32_t width = 20;
  uint32_t height = 20;
  int dx[] = {-1, 0, 1, -1, 1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  Bitmap *live = bitmap_create(width, height);
  Bitmap *live2 = bitmap_create(width, height);
  Bitmap *live3 = bitmap_create(width, height);
  srand(time(NULL));
  int ninitial_pixel = rand() % (live->height << 4);
  spawn(ninitial_pixel, live);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  printf("\033[H");
  printf("Generation 0:\n");
  print_bitmap(live);
  //  usleep(100000);
  //   epoch loop
  int max_epochs = 100000;
  for (int e = 0;; e++) {
    // hide cursor:
    printf("\033[?25l");
    size_t changes = 0;

    // define which live i will use:
    printf("\033[H");
    printf("Generation %d/%d ", e + 1, max_epochs);

    // Progress bar
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
    int vari = (e % 3) + 1;
    if (vari == 1) {
      loop(live, live2, &changes, dx, dy);
      print_bitmap(live2);
      // check for repeating pattern
    } else if (vari == 2) {
      loop(live2, live3, &changes, dx, dy);
      print_bitmap(live3);
    } else if (vari == 3) {
      loop(live3, live, &changes, dx, dy);
      print_bitmap(live);
    } else
      printf("error");
    fflush(stdout);
    bool repeating = isrepeating(live, live3);
    if (changes == 0 || repeating) {
      if (vari == 1) {
        spawn(live->width, live2);
      } else if (vari == 2) {
        spawn(live->width, live3);
      } else if (vari == 3) {
        spawn(live->width, live);
      }
    }
    if (e == max_epochs) {
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      printf("\nCompleted %d epochs in %.3f seconds (%.2f epochs/sec)\n", e,
             elapsed, e / elapsed);
      printf("\033[?25h");
      exit(0);
    }
    fflush(stdout);
    // usleep(100000);
  }
  printf("\033[?25h");
  bitmap_destroy(live);
  bitmap_destroy(live2);
  bitmap_destroy(live3);
  return 0;
}
