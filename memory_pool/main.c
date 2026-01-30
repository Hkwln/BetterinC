#include "freelist.h"
#include "particle_demo.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  FreeListPool *pool = freelist_pool_create(sizeof(Particle), 1000);
  start_sim(pool);
  freelist_pool_destroy(pool);
  return 0;
}
