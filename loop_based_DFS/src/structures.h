#pragma once

#include <stdio.h>
#include <stdlib.h>

struct Graph {
  unsigned int node_count;
  int **adj;        // adj[i] is an array of neighbour indices
  int *adj_lengths; // length of each adjacency list
  int *values;      // optional
};

extern struct Graph *gg;

void init_fixed_graph(void);
void destroy_graph(void);
void print_test(void);
void add_x_nodes(size_t number_nodes, int *lists[], int *lengths);
