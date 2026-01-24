#include "particle_demo.h"
#include <math.h>
void *spawn_new(FreeListPool *pool, Particle *p, int count) {
  p = freelist_pool_alloc(pool);
  p->x = (count * 2134) % 30;
  p->vx = cos(-count * 53);
  p->y = (count * 4312) % 30;
  p->vy = sin(-count * 53);
  p->active = 1;
  p->lifetime = (int)round(count) % 80;
  return p;
}
void frame(FreeListPool *pool, Particle **p) {
  for (int i = 0; i < 1000; i++) {
    if (p[i]->active) {
      p[i]->x += p[i]->vx;
      p[i]->y += p[i]->vy;
      p[i]->lifetime--;
      if (p[i]->lifetime >= 1)
        p[i] = 0;
    } else {
      freelist_pool_free(pool, p[i]);
      p[i] = spawn_new(pool, p[i], i);
    }
  }
}
void start_sim(FreeListPool *pool) {
  // Iterate all allocated particles
  // Move them: p->x += p->vx; p->y += p->vy;
  // Decrease lifetime
  // Free expired particles
  // Spawn new particles if under limit
  int size = 10000;
  Particle *p[size];
  for (int i = 0; i < 10000; i++) {
    p[i] = spawn_new(pool, p[i], i);
  }
  int generations = 200;
  for (int i = 0; i < generations; i++) {
    frame(pool, p);
  }
}
