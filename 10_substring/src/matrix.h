
#ifndef MATRIX_H
#define MATRIX_H

#include "dfa.h"
/*
 * make sure you free() once you don't need the transition matrix
 * */
int **construct_transition_matrix(state_t *possible_states, size_t count);

/*
 * only accepts quadratic matrices
 * */
int **matrix_mult(int **A, int **B, size_t dim);
/*
 * returns true if they are the same
 * */
bool matrix_compar(int **A, int **B, size_t dim);

#endif // MATRIX_H
