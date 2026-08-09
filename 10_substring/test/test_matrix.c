
#ifdef TEST

#include "unity.h"

#include "dfa.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

// INFO: jus a helper function
int **allocate_and_copy(int src[3][3]) {
  int **dest = malloc(3 * sizeof(int *)); // Allocate array of pointers
  for (int i = 0; i < 3; i++) {
    dest[i] = malloc(3 * sizeof(int)); // Allocate each row
    for (int j = 0; j < 3; j++) {
      dest[i][j] = src[i][j]; // Copy values
    }
  }
  return dest;
}

void setUp(void) {}

void tearDown(void) {}

void test_transition_matrix(void) {
  state_t state[7] = {{0, 0},  {2, 1},   {4, 1},  {16, 1},
                      {64, 1}, {128, 1}, {256, 1}};
  int **t_matrix = construct_transition_matrix(state, 7);
  size_t count = 0;
  for (int i = 0; i < 8; i++) {
    count += t_matrix[2][i];
  }
  TEST_ASSERT_TRUE(count == 10);

  TEST_ASSERT_TRUE(t_matrix[0][3] != 0);

#if 0
    // TODO:
  size_t size = 12288;
  state_t *out = calloc(size, sizeof(state_t));
  int count = bfs_state(out);
  int **matrix = construct_transition_matrix(out, count);
  free(out);
#endif
}

void test_matrix_multiplication(void) {

  int a[3][3] = {{2, 4, 3}, {5, 2, 34}, {2, 3, 90}};
  int b[3][3] = {{4, 9, 0}, {8, 9, 12}, {1, -3, 1}};

  int **c = allocate_and_copy(a);
  int **d = allocate_and_copy(b);

  TEST_ASSERT_TRUE(matrix_compar(c, c,
                                 3)); // check if matrix_compar() function works
  TEST_ASSERT_FALSE(matrix_compar(c, d, 3));

  int **result = matrix_mult(c, d, 3);
  // now we check if it works
  int k[3][3] = {{43, 45, 51}, {70, -39, 58}, {122, -225, 126}};
#ifdef verbose
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      printf("%d, ", result[i][j]);
    }
  }
  printf("\n");
#endif
  int **expected = allocate_and_copy(k);
  TEST_ASSERT_TRUE(matrix_compar(expected, result, 3));

  for (int i = 0; i < 3; i++) {
    free(c[i]);
    free(d[i]);
    free(result[i]);
    free(expected[i]);
  }
  free(c);
  free(d);
  free(result);
  free(expected);
}

#endif // TEST
