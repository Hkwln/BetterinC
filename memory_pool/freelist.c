#include "freelist.h"
#include <stdlib.h>

FreeListPool *freelist_pool_create(size_t object_size, size_t capacity) {
  FreeListPool *flp = malloc(sizeof(FreeListPool));
  flp->capacity = capacity;
  flp->allocated_count = 0;
  flp->object_size = object_size;
  // XXX: sizeof(int) is that the right size i need? it depends on what you want
  // to store inside the memory pool i think
  flp->memory = calloc(capacity, sizeof(int));
  // TODO: initialize the free list:

  return flp;
}
void *freelist_pool_alloc(FreeListPool *pool) { return pool->memory; }
