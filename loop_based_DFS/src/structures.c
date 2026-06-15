// INFO: First we implement a  deep field research algorithm with a fixed adj
// matrix, later add dynamic handling of the matrix via adding and deleting,
// maybe even with indices

#include "structures.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_SIZE 100

struct Graph *gg = NULL;
void init_fixed_graph(void) {
  if (gg == NULL) {
    // init graph:
    gg = malloc(sizeof(struct Graph));
    // Warum funnktioniert das nicht?!?
    gg->adj = malloc(MAX_SIZE * sizeof(int *));
    gg->adj_lengths = malloc(MAX_SIZE * sizeof(int));
    gg->node_count = 0;
  }

  // list of all nodes:
  static int n0[] = {1, 4};
  static int n1[] = {3, 4};
  static int n2[] = {0, 3, 4};
  static int n3[] = {1};       // fourth node
  static int n4[] = {0, 1, 2}; // fith node
                               // erwartbar bei start 0: 1; 4; 3; 0
  gg->adj[0] = n0;
  gg->adj_lengths[0] = 2;
  gg->adj[1] = n1;
  gg->adj_lengths[1] = 2;
  gg->adj[2] = n2;
  gg->adj_lengths[2] = 3;
  gg->adj[3] = n3;
  gg->adj_lengths[3] = 1;
  gg->adj[4] = n4;
  gg->adj_lengths[4] = 3;
  gg->node_count = 5;
  gg->values = NULL; // optional
}

void destroy_graph() {
  // just for freeeing all memory:
  free(gg->adj);
  free(gg->adj_lengths);
  free(gg);
}

// TODO: if possible add checks if length is right
bool is_valid() {
  for (size_t row = 0; row < gg->node_count; row++) {
    for (size_t number = 0; number < gg->adj_lengths[row]; number++) {
      if ((gg->adj[row][number] >= gg->node_count)) {
        printf("this list is not valid, especially at node: %zu and number: "
               "%zu\nyour number exeeds the maximum nodes\n",
               row, number);
        return false;
      }
    }
  }
  return true;
}

void add_x_nodes(size_t number_nodes, int *lists[], int *lengths) {
  if (gg == NULL) {
    // init graph:
    gg = malloc(sizeof(struct Graph));
    // Warum funnktioniert das nicht?!?
    gg->adj = malloc(MAX_SIZE * sizeof(int *));
    gg->adj_lengths = malloc(MAX_SIZE * sizeof(int));
    gg->node_count = 0;
  }
  // There you can add all valid lists with the valid lengths
  for (int b = 0; b < number_nodes; b++) {
    if (gg->node_count > MAX_SIZE) {
      printf("error:)");
      break;
    }
    gg->adj[gg->node_count] = lists[b];
    gg->adj_lengths[gg->node_count] = lengths[b];
    gg->node_count++;
    if (!is_valid()) {
      break;
    }
  }
}
void print_test(void) {
  printf("first row of adjacent list:\n");
  for (int row = 0; row < gg->node_count; row++) {
    for (int i = 0; i < gg->adj_lengths[row]; i++) {
      printf("%d, ", gg->adj[row][i]);
    }
    printf("\n");
  }
  printf("\n");
  printf("count = %d\n", gg->node_count);
}
