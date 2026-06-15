#include "dfs.h"

// if (!visited[neighbour]))
void push(int node, int *stack, int *top, int *visited) {
  visited[node] = 1;
  stack[++(*top)] = node;
}
// call it only !is_empty(top)
int pop(int *stack, int *top) { return stack[(*top)--]; }

int is_empty(int top) { return top == -1; }

void dfs(void) {
  if (gg == NULL) {
    printf("error, you should init the struct before you call the push "
           "funciton\n");
    return;
  }
  // Define stack
  int *visited = calloc(gg->node_count, sizeof(int));
  int *stack = malloc(gg->node_count * sizeof(int));
  int top = -1;
  // pushe die ereste node auf de stack
  push(1, stack, &top, visited);
  // what is the initial start loop the stack
  while (top >= 0) {
    int node = pop(stack, &top);
    visited[node] = 1;
    for (int a = 0; a < gg->adj_lengths[node]; a++) {
      // if gg->adj[i][a] is not in the stack -> append it to the stack
      int neigbour = gg->adj[node][a];
      if (visited[neigbour] == 0) {
        printf("pushed: %d ", neigbour);
        push(neigbour, stack, &top, visited);
      }
    }
  }
  printf("\n");
  free(visited);
  free(stack);
}
