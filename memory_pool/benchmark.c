#include "bitmaplist.h"
#include "freelist.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
/* TODO:*/
void benchmark_malloc(size_t count) {
  clock_t start = clock();
  size_t object_count = 1000;
  char *pizza[count];
  // allocating memory
  for (int i = 0; i < count; i++) {
    pizza[i] = malloc(object_count);
    if (pizza[i] == NULL)
      perror(pizza[i]);
  }
  // freeing the memory
  for (int i = 0; i < count; i++) {
    free(pizza[i]);
  }
  clock_t end = clock();
  double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("time taken for malloc: %f\n", time_taken);
}
void benchmark_bit_pool(BitmapPool *bmp, size_t count) {
  clock_t start = clock();
  char *pizza[count];
  for (int i = 0; i < count; i++) {
    pizza[i] = bitmap_pool_alloc(bmp);
  }
  clock_t end = clock();
  double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("time taken for bitmap: %f\n", time_taken);
}
void benchmark_freelist_pool(FreeListPool *fs, size_t count) {
  clock_t start = clock();
  char *pizza[count];
  for (int i = 0; i < count; i++) {
    pizza[i] = freelist_pool_alloc(fs);
  }
  clock_t end = clock();
  double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("time taken for freelist : %f\n", time_taken);
}

int main() {
  size_t count = 5000;
  benchmark_malloc(count);
#if 1
  BitmapPool *pool = bitmap_pool_create(1000, count);
  benchmark_bit_pool(pool, count);
  bitmap_pool_destroy(pool);
#endif
#if 1
  FreeListPool *fs = freelist_pool_create(1000, count);
  benchmark_freelist_pool(fs, count);
  freelist_pool_destroy(fs);
#endif
}
/*RESULT:
 * with count = 5000 and size =1000
 * malloc ca. 0.00527
 * bitmap:
 * -without ffs: 0.167405
 * -with ffs: 0.008882
 * freelist ca. 0.000829
 * */
