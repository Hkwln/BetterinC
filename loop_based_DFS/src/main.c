
#include "structures.h"

int main(void) {

  init_fixed_graph();
  print_test();
  int nfirst[] = {3, 1};
  int nsecound[] = {1, 5};
  int *list[] = {nfirst, nsecound};
  int lengths[] = {2, 2};
  add_x_nodes(2, list, lengths);
  print_test();
  destroy_graph();
  return 0;
}
