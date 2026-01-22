#include "bitmaplist.h"
#include "freelist.h"
#include <time.h>
/* TODO:*/
void benchmark_malloc(size_t count);
void benchmark_bit_pool(BitmapPool *bmp, size_t count);
void benchmark_freelist_pool(FreeListPool *fs, size_t count);

int main() {
  size_t count;
  benchmark_malloc(count);
}
