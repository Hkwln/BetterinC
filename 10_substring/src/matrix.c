
#include "matrix.h"
#include <stdlib.h>
// now is finally the time to start working on matrix stuff --- :)

int **construct_transition_matrix(state_t *possible_states, size_t count) {
  int row = count, cols = count;
  int **t_matrix = malloc(row * sizeof(int *));
  for (int i = 0; i < row; i++) {
    t_matrix[i] = calloc(cols, sizeof(int));
  }

  int v_index = -1;
  size_t u = 0;
  for (int i = 0; i < count; i++) {
    for (int v = 0; v < 10; v++) {
      state_t state_nxt = state_next(possible_states[i], v);
      for (int j = 0; j < 9; j++) {
        if (state_equal(state_nxt, possible_states[j])) {
          v_index = j;
          break;
        }
      }
      if (v_index != -1)
        t_matrix[i][v_index] += 1;
    }
  }
  return t_matrix;
}

int **matrix_mult(int **A, int **B, size_t dim) {
  int **c = malloc(dim * sizeof(int *));
  for (size_t i = 0; i < dim; i++) {
    c[i] = calloc(dim, sizeof(int));
  }

  for (int i = 0; i < dim; i++) {
    for (size_t j = 0; j < dim; j++) {
      c[i][j] = 0;
      for (size_t k = 0; k < dim; k++) {
        c[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  return c;
}
// INFO: just a helper function
bool matrix_compar(int **A, int **B, size_t dim) {
  for (size_t i = 0; i < dim; i++) {
    for (size_t j = 0; j < dim; j++) {
      if (A[i][j] != B[i][j]) {
        return false;
      }
    }
  }

  return true;
}
