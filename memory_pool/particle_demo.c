#include "particle_demo.h"
#include <math.h>
#include <stdlib.h>
// TODO: change from normal iteration(count to 1000 and check if lifetime = 0)
// to circular buffer
Particle *spawn_new(FreeListPool *pool, ParticleRingBuffer *cp) {
  Particle *p = freelist_pool_alloc(pool);
  p->x = (cp->count * 2134) % 30;
  p->vx = cos(-cp->count * 53);
  p->y = (cp->count * 4312) % 30;
  p->vy = sin(-cp->count * 53);
  p->lifetime = (size_t)round(cp->count * 1245) % 80;
  return p;
}
void frame(FreeListPool *pool, ParticleRingBuffer *cp) {
  size_t pos = cp->head;
  // check if head particle is dead
  while (cp->count > 0 && cp->particle[cp->head]->lifetime <= 0) {
    freelist_pool_free(pool, cp->particle[cp->head]);
    cp->head = (cp->head + 1) % cp->capacity;
    cp->count--;
  }
  // update all active particles
  for (size_t i = 0; i < cp->count; i++) {
    pos = (cp->head + i) % cp->capacity;
    cp->particle[pos]->x += cp->particle[pos]->vx;
    cp->particle[pos]->y += cp->particle[pos]->vy;
    cp->particle[pos]->lifetime--;
  }
  // spawn new particles if space available
  while (cp->count < cp->capacity) {
    cp->particle[cp->tail] = spawn_new(pool, cp);
    cp->tail = (1 + cp->tail) % cp->capacity;
    cp->count += 1;
  }
}
void start_sim(FreeListPool *pool) {
  // Iterate all allocated particles
  // Move them: p->x += p->vx; p->y += p->vy;
  // Decrease lifetime
  // Free expired particles
  // Spawn new particles if under limit
  ParticleRingBuffer *cp = malloc(sizeof(ParticleRingBuffer));
  cp->capacity = 1000;
  cp->count = 0;
  cp->head = 0;
  cp->tail = 0;
  Particle *p[cp->capacity];
  cp->particle = p;
  // initial spawning
  do {
    p[cp->tail] = spawn_new(pool, cp);
    // XXX: styleissue? dublicates what I do elsewhere?
    cp->tail = (1 + cp->tail) % cp->capacity;
    cp->count += 1;
  } while (cp->tail != 0);
  // actual epoch simulation
  int generations = 200;
  for (int i = 0; i < generations; i++) {
    frame(pool, cp);
    if (cp->count > 0) {
      printf("Gen %d - First particle: x=%.2f, count= %zu, lifetime=%.2f\n", i,
             cp->particle[cp->head]->x, cp->count,
             cp->particle[cp->head]->lifetime);
    }
  }
  free(cp);
}
