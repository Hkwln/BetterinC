#include "dfs.h"
// TODO:
void push(void) {
  if (gg == NULL) {
    printf("error, you should init the struct before you call the push "
           "funciton\n");
    return;
  }
  // Define stack
  int stack[gg->node_count];
  int append = 0;
  int *visited = calloc(gg->node_count, sizeof(int));
  int i = 0;
  // what is the initial start stack
  do {
    // Visited check
    if (visited[i] == 0) {
      // mark the node as visited
      visited[i] = 1;
      // push the node onto the stack
      // quasi neue möglichkeiten
      for (int a = 0; a < gg->adj_lengths[i]; a++) {
        // if gg->adj[i][a] is not in the stack -> append it to the stack
        int not = 0;
        for (int b = 0; b <= gg->node_count; b++) {
          if (gg->adj[i][a] == stack[b]) {
            not++;
          }
        }
        if (not >= gg->node_count) {
          stack[append++] = gg->adj[i][a];
        }
      }
    }
    i++;
  } while (i <= gg->node_count);
  free(visited);
}

void pop(void); // removes the node and returns the top noode
void is_empty();
