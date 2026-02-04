#include "freelist.h"
#include <stdlib.h>

FreeListPool *freelist_pool_create(size_t object_size, size_t capacity) {
  FreeListPool *flp = malloc(sizeof(FreeListPool));
  flp->capacity = capacity;
  flp->allocated_count = 0;
  flp->object_size = object_size;
  flp->memory = calloc(capacity, object_size);
  flp->freeList = (Poolnode *)flp->memory;
  Poolnode *current = flp->freeList;
  for (int i = 0; i < capacity - 1; i++) {
    void *nextaddr = (char *)flp->memory + (i + 1) * object_size;
    current->next = nextaddr;
    current = current->next;
  }
  return flp;
}
void *freelist_pool_alloc(FreeListPool *pool) {
  if (pool->freeList == NULL)
    return NULL;
  void *result = pool->freeList;
  pool->freeList = pool->freeList->next;
  pool->allocated_count++;
  return result;
}
// FIXME: add bound checking:
void freelist_pool_free(FreeListPool *pool, void *ptr) {
  Poolnode *node = (Poolnode *)ptr;
  node->next = pool->freeList;
  pool->freeList = node;
  pool->allocated_count--;
}
void freelist_pool_destroy(FreeListPool *flp) {
  flp->capacity = 0;
  flp->allocated_count = 0;
  flp->object_size = 0;
  free(flp->memory);
  free(flp);
}
