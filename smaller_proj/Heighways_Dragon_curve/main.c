#include "render_the_dragon.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  size_t n = 15;
  if (argc > 1) {
    char *end = NULL;
    unsigned long long v = strtoull(argv[1], &end, 10);
    if (end == argv[1] || v == 0) {
      fprintf(stderr, "usage: %s [generations 1..62]\n", argv[0]);
      return 1;
    }
    n = (size_t)v;
  }
  if (n > 62) {
    fprintf(stderr, "generations must be <= 62 (2^62 segments is beyond "
                    "feasible anyway)\n");
    return 1;
  }
  if (render_to_svg(n, "dragon.svg")) {
    printf("order-%zu dragon written to dragon.svg (%llu segments, "
           "O(1) memory)\n",
           n, 1ULL << n);
    return 0;
  }
  fprintf(stderr, "failed to write dragon.svg\n");
  return 1;
}
