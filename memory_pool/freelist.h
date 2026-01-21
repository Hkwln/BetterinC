#pragma once
#include <stdio.h>
// XXX: was passiert hier, ich check nicht ganz warum ich in dem poolnode objekt
// das nächste poolnode objekt store, also nicht warum freelist, sondern warum
// und wie funktioniert das in C?
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
