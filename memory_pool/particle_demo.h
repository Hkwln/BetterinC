#pragma once
#include "freelist.h"
#include <stdbool.h>

typedef struct {
  double x, y;
  double vx, vy;
  float lifetime;
  bool active;
} Particle;
void start_sim(FreeListPool *pool);
