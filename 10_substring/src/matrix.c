
#include "matrix.h"
#include <stdlib.h>
// now is finally the time to start working on matrix stuff --- :)

int **construct_transition_matrix(state_t *possible_states, size_t count) {
  // state_hash is injective over (F, C, fresh): F, C in 0..1023, fresh in 0..1
  // -> 0..2^21-1, so a plain array maps a state's hash directly to its
  // position in the list -- no collision handling needed.
  int *hash_to_index = malloc((1u << 21) * sizeof(int));
  for (size_t h = 0; h < (1u << 21); h++) {
    hash_to_index[h] = -1;
  }
  for (size_t i = 0; i < count; i++) {
    hash_to_index[state_hash(possible_states[i])] = (int)i;
  }

  int **t_matrix = malloc(count * sizeof(int *));
  for (size_t i = 0; i < count; i++) {
    t_matrix[i] = calloc(count, sizeof(int));
  }

  for (size_t i = 0; i < count; i++) {
    for (int v = 0; v < 10; v++) {
      state_t nxt = state_next(possible_states[i], v);
      int target = hash_to_index[state_hash(nxt)];
      if (target != -1) // skip if the list is not transition-closed
        t_matrix[i][target] += 1;
    }
  }
  free(hash_to_index);
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
