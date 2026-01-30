#pragma once
#include "freelist.h"
#include <stdbool.h>

typedef struct {
  double x, y;
  double vx, vy;
  float lifetime;
} Particle;
// Circular buffer:
typedef struct {
  Particle **particle;
  size_t head;
  size_t tail;
  size_t count;
  size_t capacity;
} ParticleRingBuffer;
void start_sim(FreeListPool *pool);
