# Coding Exercise: Memory Pool Allocator in C

**Difficulty:** Advanced  
**Topic:** Memory management + Linked lists + Bit operations + Performance optimization  
**Estimated Time:** 10-16 hours (2 days with breaks)

---

## Background

A **memory pool** (object pool) pre-allocates a fixed number of objects and reuses them, avoiding repeated malloc/free calls. Unlike your arena allocator (which never frees individual objects), a pool tracks which objects are free and which are in use.

**Used in:**
- Game engines (bullet pools, particle systems)
- Database connection pools
- Network servers (request/response object reuse)
- Real-time systems (predictable performance, no malloc in hot path)

**Key difference from arena:**
- Arena: Allocate sequentially, reset all at once
- Pool: Allocate/free individual objects, reuse slots

---

## Part 1: Understanding the Problem (Read carefully!)

### Why Memory Pools?

```c
// BAD: Constant malloc/free in game loop (slow, fragmentation)
for (int i = 0; i < 1000; i++) {
    Bullet* b = malloc(sizeof(Bullet));  // Expensive!
    // ... use bullet ...
    free(b);  // Fragmentation risk!
}

// GOOD: Reuse pre-allocated objects
Pool* pool = pool_create(sizeof(Bullet), 1000);
for (int i = 0; i < 1000; i++) {
    Bullet* b = pool_alloc(pool);  // O(1) pointer return!
    // ... use bullet ...
    pool_free(pool, b);  // Just marks as free
}
```

### Two Implementation Strategies

You'll implement **both** and compare performance:

**Strategy A: Bitmap Tracking** (Uses your bitmap knowledge!)
- Track free slots with bits: 0=used, 1=free
- Fast allocation: Find first set bit (ffs instruction)
- Cache-friendly metadata

**Strategy B: Free List** (Classic approach)
- Embed a linked list in the free objects themselves
- O(1) allocation from list head
- Zero metadata overhead

---

## Part 2: Data Structures (30 minutes)

### Strategy A: Bitmap Pool

```c
typedef struct {
    void* memory;           // The big block of objects
    uint8_t* free_bitmap;   // 1 bit per object (1=free)
    size_t object_size;     // Size of each object
    size_t capacity;        // Total number of objects
    size_t allocated_count; // Currently allocated
} BitmapPool;
```

**Memory layout:**
```
[OBJ][OBJ][OBJ][OBJ]...  <- Actual objects
Bitmap: 11010110...      <- Free status
         ↑
       First free object at index 0
```

### Strategy B: Free List Pool

```c
typedef struct PoolNode {
    struct PoolNode* next;
} PoolNode;

typedef struct {
    void* memory;
    PoolNode* free_list;    // Head of free objects list
    size_t object_size;
    size_t capacity;
    size_t allocated_count;
} FreeListPool;
```

**Memory layout (initially):**
```
[NODE→][NODE→][NODE→][NULL]
  ↑
free_list

Each free object contains a pointer to the next free object!
```

---

## Part 3: Bitmap Pool Implementation (3-4 hours)

### Function 1: Create Bitmap Pool

```c
BitmapPool* bitmap_pool_create(size_t object_size, size_t capacity);
```

**Steps:**
1. Allocate memory for pool struct
2. Allocate memory block: `capacity * object_size` bytes
3. Calculate bitmap size: `(capacity + 7) / 8` bytes
4. Allocate bitmap with `calloc` (zeros = all used initially)
5. Set all bits to 1 (all free): Use `memset(bitmap, 0xFF, bitmap_size)`
6. Return pool

**Edge case:** Ensure `object_size >= sizeof(void*)` (you'll see why later!)

### Function 2: Allocate from Bitmap Pool

```c
void* bitmap_pool_alloc(BitmapPool* pool);
```

**Algorithm:**
1. Check if pool is full: `if (allocated_count == capacity) return NULL;`
2. Find first free object:
   ```c
   for (size_t byte_idx = 0; byte_idx < bitmap_bytes; byte_idx++) {
       if (pool->free_bitmap[byte_idx] != 0) {
           // Found a byte with free bits!
           uint8_t byte = pool->free_bitmap[byte_idx];
           // Find first set bit (use your bit operation knowledge!)
           int bit_idx = __builtin_ffs(byte) - 1;  // GCC builtin
           // Or write your own: for (int i = 0; i < 8; i++) if (byte & (1 << i))
           size_t object_idx = byte_idx * 8 + bit_idx;
           // Clear the bit (mark as used)
           pool->free_bitmap[byte_idx] &= ~(1 << bit_idx);
           pool->allocated_count++;
           // Return pointer to object
           return (uint8_t*)pool->memory + (object_idx * pool->object_size);
       }
   }
   return NULL;  // Should never reach if count check is correct
   ```

**Optimization challenge:** Can you avoid the loop by caching the last allocated index?

### Function 3: Free to Bitmap Pool

```c
void bitmap_pool_free(BitmapPool* pool, void* ptr);
```

**Steps:**
1. Calculate object index:
   ```c
   size_t offset = (uint8_t*)ptr - (uint8_t*)pool->memory;
   size_t object_idx = offset / pool->object_size;
   ```
2. Validate: Check `offset % object_size == 0` (detect invalid pointers!)
3. Check if already free (double-free detection!)
4. Set bit to 1 (mark as free)
5. Decrement `allocated_count`

**Security check:** Add assertion that `object_idx < capacity`

### Function 4: Destroy Bitmap Pool

```c
void bitmap_pool_destroy(BitmapPool* pool);
```

Free in correct order!

---

## Part 4: Free List Pool Implementation (3-4 hours)

### Function 1: Create Free List Pool

```c
FreeListPool* freelist_pool_create(size_t object_size, size_t capacity);
```

**Crucial step:** Initialize the free list!

```c
// After allocating memory block:
pool->free_list = (PoolNode*)pool->memory;

// Thread all objects together
PoolNode* current = pool->free_list;
for (size_t i = 0; i < capacity - 1; i++) {
    void* next_addr = (uint8_t*)current + object_size;
    current->next = (PoolNode*)next_addr;
    current = current->next;
}
current->next = NULL;  // Last object points to NULL
```

**Why `object_size >= sizeof(void*)`?** Because free objects store a pointer!

### Function 2: Allocate from Free List

```c
void* freelist_pool_alloc(FreeListPool* pool);
```

**Beautifully simple:**
```c
if (pool->free_list == NULL) return NULL;

void* result = pool->free_list;
pool->free_list = pool->free_list->next;  // Pop from list
pool->allocated_count++;
return result;
```

**That's it!** O(1) with just 4 lines of code.

### Function 3: Free to Free List

```c
void freelist_pool_free(FreeListPool* pool, void* ptr);
```

```c
PoolNode* node = (PoolNode*)ptr;
node->next = pool->free_list;  // Insert at head
pool->free_list = node;
pool->allocated_count--;
```

**Even simpler!** O(1), no searching needed.

### Function 4: Destroy Free List Pool

Same as bitmap version.

---

## Part 5: Advanced Features (2-3 hours)

### Feature 1: Debug Mode - Detect Use-After-Free

```c
void bitmap_pool_free(BitmapPool* pool, void* ptr) {
    // ... existing code ...
    #ifdef DEBUG_POOL
    memset(ptr, 0xDD, pool->object_size);  // Fill with poison value
    #endif
}
```

If someone accesses freed memory, they'll read `0xDD` bytes!

### Feature 2: Pool Statistics

```c
typedef struct {
    size_t total_allocations;
    size_t total_frees;
    size_t peak_usage;
    size_t current_usage;
} PoolStats;

void pool_print_stats(BitmapPool* pool);
```

Track allocation patterns for optimization.

### Feature 3: Thread-Local Pools (CHALLENGE)

For multi-threaded games/servers, each thread gets its own pool:

```c
_Thread_local FreeListPool* thread_pool;
```

No locks needed!

### Feature 4: Defragmentation (HARD)

Compact allocated objects to reduce memory footprint. Requires:
- Track all pointers to objects
- Move objects in memory
- Update all references

This is what garbage collectors do!

---

## Part 6: Performance Testing (2-3 hours)

### Test 1: Benchmark Against malloc

```c
#include <time.h>

void benchmark_malloc(size_t count) {
    clock_t start = clock();
    for (size_t i = 0; i < count; i++) {
        void* p = malloc(64);
        free(p);
    }
    clock_t end = clock();
    printf("malloc: %.3f ms\n", (end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void benchmark_pool(BitmapPool* pool, size_t count) {
    clock_t start = clock();
    for (size_t i = 0; i < count; i++) {
        void* p = bitmap_pool_alloc(pool);
        bitmap_pool_free(pool, p);
    }
    clock_t end = clock();
    printf("pool: %.3f ms\n", (end - start) * 1000.0 / CLOCKS_PER_SEC);
}
```

**Expected:** Pool should be 10-100x faster!

### Test 2: Fragmentation Test

```c
// Allocate 1000 objects, free every other one
// Try to allocate 500 more
// With malloc: Might fail due to fragmentation
// With pool: Always succeeds (predictable memory)
```

### Test 3: Cache Performance

Use `perf stat` on Linux to measure cache misses:

```bash
perf stat -e cache-misses,cache-references ./pool_test
```

Bitmap vs Free List: Which has better cache locality?

---

## Part 7: Real-World Application (3-4 hours)

### Build a Particle System Simulator

**Requirements:**
- 10,000 particles
- Each particle: position (x,y), velocity (vx,vy), lifetime
- Every frame: Move particles, expire old ones, spawn new ones
- Measure FPS with pool vs malloc

```c
typedef struct {
    float x, y;
    float vx, vy;
    float lifetime;
} Particle;

void update_particles(FreeListPool* pool) {
    // Iterate all allocated particles
    // Move them: p->x += p->vx; p->y += p->vy;
    // Decrease lifetime
    // Free expired particles
    // Spawn new particles if under limit
}
```

**Challenge:** How do you iterate only allocated objects?

**Solution:** Keep a separate array of active pointers:
```c
Particle* active[10000];
size_t active_count;
```

Or use your **circular buffer** to track active particles! 🚀

---

## Part 8: Integration with Your Previous Work (Optional bonus)

### Use Your String Builder for Pool Names

```c
typedef struct {
    FreeListPool base;
    StringBuilder* name;  // Pool identifier
    StringBuilder* debug_log;
} NamedPool;

void pool_log(NamedPool* p, const char* msg) {
    sb_append_format(p->debug_log, "[%s] %s\n", 
                     sb_get_string(p->name), msg);
}
```

### Use Your Hash Table for Multi-Pool Manager

```c
typedef struct {
    HashTable* pools;  // Key: pool name, Value: pool pointer
} PoolManager;

void* manager_alloc(PoolManager* mgr, const char* pool_name);
```

This teaches you how **data structures compose**!

---

## Testing Checklist

- [ ] Both implementations compile without warnings
- [ ] Can allocate all objects until full
- [ ] Free and re-allocate works correctly
- [ ] Double-free detected (with assertion)
- [ ] Invalid pointer detected (offset check)
- [ ] No memory leaks (valgrind clean)
- [ ] Pool faster than malloc (benchmark)
- [ ] Works with object_size = 8, 64, 1024
- [ ] Works with capacity = 10, 10000
- [ ] Thread-safe version (if implemented)

---

## What You'll Learn

✓ **Object pooling pattern** (game dev essential)  
✓ **Bit manipulation for metadata** (embedded systems technique)  
✓ **Intrusive data structures** (free list in objects)  
✓ **Memory layout optimization** (cache effects)  
✓ **Performance profiling** (perf, valgrind)  
✓ **Trade-offs:** Bitmap vs Free List  
✓ **Real-time constraints** (no malloc in hot path)  
✓ **Combining data structures** (integration with your work)

---

## Hints

- **Start with free list** - It's simpler!
- **Draw the memory layout** - Visualize object placement
- **Test with struct size = 16** - Easy to debug
- **Print the free list** - Walk and print all free nodes
- **Use assertions liberally** - Catch bugs early
- **Benchmark with -O3** - Optimizations matter here
- **Read `__builtin_ffs` docs** - GCC bit operation builtins

---

## Common Pitfalls

⚠️ **Forgetting to initialize free list** - All objects must be linked!  
⚠️ **object_size < sizeof(void*)** - Free list won't fit!  
⚠️ **Not checking for NULL** - Always validate pool_alloc return  
⚠️ **Pointer arithmetic errors** - uint8_t* for byte offsets!  
⚠️ **Bitmap byte order** - Bit 0 is LSB, bit 7 is MSB  
⚠️ **Cache effects** - Sequential access is faster than random

---

## Extension Ideas

1. **Buddy allocator:** Split/merge power-of-2 blocks
2. **Slab allocator:** What Linux kernel uses
3. **Growing pools:** Auto-expand when full
4. **Memory-mapped pools:** Use `mmap` instead of `malloc`
5. **Cross-process pools:** Shared memory between processes
6. **SIMD optimization:** Scan bitmap with AVX2
7. **Lock-free pools:** Compare-and-swap for threads

---

## Why This Exercise?

This is **significantly more advanced** than your previous work:

1. **String builder/Hash table:** Single pattern
2. **This exercise:** Two competing implementations + benchmarking
3. **Teaches:** Performance analysis, trade-off decisions
4. **Real use:** Every game engine uses this
5. **Combines:** Bits, pointers, linked lists, performance profiling

You'll understand why Redis, SQLite, and game engines don't use malloc in hot code!

---

## Submission Files

```
memory_pool/
├── bitmap_pool.h
├── bitmap_pool.c
├── freelist_pool.h
├── freelist_pool.c
├── benchmark.c
├── particle_demo.c
├── Makefile
└── README.md (your performance comparison!)
```

In README.md, answer:
- Which implementation is faster? Why?
- Which uses less memory? Why?
- When would you use each?
- What did you learn about CPU caches?

**This is your most advanced C exercise yet. Take your time and enjoy learning!** 🚀
