#include <stdio.h>

typedef struct {
  size_t total_allocations;
  size_t total_frees;
  size_t peak_usage;
} PoolStats;
