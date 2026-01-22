#pragma once
#include <stdio.h>
typedef struct Poolnode {
  struct Poolnode *next;
} Poolnode;
// XXX: objects are always the same size
typedef struct {
  void *memory;
  Poolnode *freeList; // Head of free List
  size_t object_size;
  size_t capacity;
  size_t allocated_count;
} FreeListPool;
FreeListPool *freelist_pool_create(size_t object_size, size_t capacity);
void *freelist_pool_alloc(FreeListPool *pool);
void freelist_pool_free(FreeListPool *pool, void *ptr);
void freelist_pool_destroy(FreeListPool *pool);
// TODO:
void fbool_print_stats(FreeListPool *pool);
