// TODO:ziel: switche langsam zu ly von der mitte aus nach außen, game of live
// oberfläche wird quasi immer kleiner , also in tty, nicht im code,
#include "bitmap.h"
#include "data.h"
#include "draw.h"
#include <asm-generic/ioctls.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
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
// Dixplay in tty
volatile sig_atomic_t running = 1;
void sigterm_handler(int sig) { running = 0; }
int main() {
  bool manual = false;
#if !manual
  signal(SIGTERM, sigterm_handler);
  signal(SIGINT, sigterm_handler);
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  uint32_t width = w.ws_col - 1;  // -1 damit keine Zeile umbricht!
  uint32_t height = w.ws_row - 3; // -3 für Header + Rand
#endif
  // init:
#if manual
  uint32_t width = 20;
  uint32_t height = 20;
#endif
  int max_epochs = 500;
  int dx[] = {-1, 0, 1, -1, 1, -1, 0, 0};
  int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  Bitmap *live = bitmap_create(width, height);
  Bitmap *live2 = bitmap_create(width, height);
  Bitmap *live3 = bitmap_create(width, height);
  Active *active = initactive(live->height * live->width);
  Active *active2 = initactive(live->height * live->width);
  Active *active3 = initactive(live->height * live->width);
  srand(time(NULL));
  int ninitial_pixel = rand() % (live->height << 5);
  // Initial random spawn across entire grid
  for (int i = 0; i < ninitial_pixel; i++) {
    int randomx = rand() % live->width;
    int randomy = rand() % live->height;
    bitmap_set_pixel(live, randomx, randomy, 1);
  }
  set_Aactive(active, live, dx, dy);
  print_bitmap(live);
  usleep(100000);
  // start timer:
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Box-Animation initialisieren (zentriert)
  int box_width = width / 2;
  int box_height = height / 2;
  int box_x = (width - box_width) / 2;
  int box_y = (height - box_height) / 2;
  BoxAnimation box = box_anim_init(box_x, box_y, box_width, box_height);

#if manual
  printf("\033[H");
  printf("Generation 0:\n");
#endif
  // epoch loop
  for (int e = 0;; e++) {
    int changes = 0;
#if manual
    printf("\033[?25l");
    printf("\033[H");

    printf("Generation %d/%d ", e + 1, max_epochs);

    // Progress bar (from data.h)
    progress_bar(max_epochs, e);
#endif
    // Actuall Convway epoch loop with printing
    int vari = (e % 3) + 1;
    if (vari == 1) {
      changes = loop(live, live2, active, active2, dx, dy);
      box_anim_draw_frame(&box, live2, 2); // Update box state
      print_bitmap_with_box(live2, &box);  // Print mit Box-Overlay
    } else if (vari == 2) {
      changes = loop(live2, live3, active2, active3, dx, dy);
      box_anim_draw_frame(&box, live3, 2);
      print_bitmap_with_box(live3, &box);
    } else if (vari == 3) {
      changes = loop(live3, live, active3, active, dx, dy);
      box_anim_draw_frame(&box, live, 2);
      print_bitmap_with_box(live, &box);
    }
    // XXX: only when sleep timer is set to 0 necessary
    // fflush(stdout);

    // Count alive cells from the active set
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
    uint max_alive = (uint)(((50 + e) * 2) >> e % 10);
    uint spawn_number = live->width << 3;
    if (manual) {
      max_alive = 10;
      spawn_number = live->width * 2;
    }
    if (changes == 0 || alive < max_alive) {

      if (vari == 1) {
        spawn(spawn_number, live2, active2);
        set_Aactive(active2, live2, dx, dy);
      } else if (vari == 2) {
        spawn(spawn_number, live3, active3);
        set_Aactive(active3, live3, dx, dy);
      } else if (vari == 3) {
        spawn(spawn_number, live, active);
        set_Aactive(active, live, dx, dy);
      }
    }
    // exit

    if (e == max_epochs) {
      clock_gettime(CLOCK_MONOTONIC, &end);
      double elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
      printf("\nCompleted %d epochs in %.3f seconds (%.2f epochs/sec)\n", e,
             elapsed, e / elapsed);
      printf("\033[?25h");
      exit(0);
    }
    usleep(20000);
  }
#if manual
  printf("\033[?25h");
#else
  printf("\033[2J\033[H"); // Clear screen
#endif

  // Cleanup Box-Animation
  box_anim_free(&box);

  bitmap_destroy(live);
  bitmap_destroy(live2);
  bitmap_destroy(live3);
  destroyactive(active);
  destroyactive(active2);
  destroyactive(active3);
  return 0;
}
