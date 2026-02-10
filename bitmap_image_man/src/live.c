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
/* Spawns new pixels - randomly when dead, or near active areas when count is
 * low*/
void spawn(size_t count, Bitmap *live, Active *active) {
  for (int i = 0; i < count; i++) {
    int x, y;
    // If we have active cells, spawn near them (70% chance)
    if (active->count > 0 && rand() % 100 < 70) {
      int random_idx = rand() % active->count;
      int idx = active->indices[random_idx];
      x = idx % live->width;
      y = idx / live->width;
    } else {
      // Otherwise spawn randomly anywhere
      x = rand() % live->width;
      y = rand() % live->height;
    }
    bitmap_set_pixel(live, x, y, 1);
  }
}
/*Checks and returnshow many neighbors the current pixel has;*/
int getnachbarn(Bitmap *live, uint32_t x, uint32_t y, int dx[], int dy[]) {
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
int loop(Bitmap *live, Bitmap *live2, Active *active, Active *next_active,
         int dx[], int dy[]) {
  // Clear destination bitmap
  int changes = 0;
  for (int y = 0; y < live2->height; y++) {
    for (int x = 0; x < live2->width; x++) {
      bitmap_set_pixel(live2, x, y, 0);
    }
  }

  reset_active(next_active, live2);

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

// Count how many cells are actually alive in the bitmap
int count_alive(Bitmap *bmp) {
  int count = 0;
  for (int y = 0; y < bmp->height; y++) {
    for (int x = 0; x < bmp->width; x++) {
      if (bitmap_get_pixel(bmp, x, y))
        count++;
    }
  }
  return count;
}
// XXX: vllt lasse den user die göße und die schnelligkeit bestimmen?
int main(int argc, char **argv) {
  uint32_t width = 50;
  uint32_t height = 50;
  int dx[] = {-1, 0, 1, -1, 1, -1, 0, 0};
  int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  Bitmap *live = bitmap_create(width, height);
  Bitmap *live2 = bitmap_create(width, height);
  Bitmap *live3 = bitmap_create(width, height);
  Active *active = initactive(live->height * live->width);
  Active *active2 = initactive(live->height * live->width);
  Active *active3 = initactive(live->height * live->width);

  srand(time(NULL));
  int ninitial_pixel = rand() % (live->height << 4);

  // Initial random spawn across entire grid
  for (int i = 0; i < ninitial_pixel; i++) {
    int randomx = rand() % live->width;
    int randomy = rand() % live->height;
    bitmap_set_pixel(live, randomx, randomy, 1);
  }

  set_Aactive(active, live, dx, dy);
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  printf("\033[H");
  printf("Generation 0:\n");
  print_bitmap(live);
  usleep(100000);
  //    epoch loop
  int max_epochs = 100000;
  for (int e = 0;; e++) {
    printf("\033[?25l");
    int changes = 0;
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
      changes = loop(live, live2, active, active2, dx, dy);
      print_bitmap(live2);
    } else if (vari == 2) {
      changes = loop(live2, live3, active2, active3, dx, dy);
      print_bitmap(live3);
    } else if (vari == 3) {
      changes = loop(live3, live, active3, active, dx, dy);
      print_bitmap(live);
    } else {
      printf("error");
    }
    fflush(stdout);

    // Count alive cells from the active set (more efficient than scanning whole
    // bitmap)
    Active *current_active =
        (vari == 1) ? active2 : ((vari == 2) ? active3 : active);
    Bitmap *current_bmp = (vari == 1) ? live2 : ((vari == 2) ? live3 : live);
    int alive = 0;
    for (int i = 0; i < current_active->count; i++) {
      int x = current_active->indices[i] % current_bmp->width;
      int y = current_active->indices[i] / current_bmp->width;
      if (bitmap_get_pixel(current_bmp, x, y))
        alive++;
    }

    // Spawn when: no changes (stuck/still life) OR very low alive count
    if (changes == 0 || alive < 10) {
      if (vari == 1) {
        spawn(live->width * 2, live2, active2);
        set_Aactive(active2, live2, dx, dy);
      } else if (vari == 2) {
        spawn(live->width * 2, live3, active3);
        set_Aactive(active3, live3, dx, dy);
      } else if (vari == 3) {
        spawn(live->width * 2, live, active);
        set_Aactive(active, live, dx, dy);
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
    usleep(50000);
  }
  printf("\033[?25h");
  bitmap_destroy(live);
  bitmap_destroy(live2);
  bitmap_destroy(live3);
  destroyactive(active);
  destroyactive(active2);
  destroyactive(active3);
  return 0;
}
