
#ifdef TEST

#include "unity.h"

#include "dfa.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

// INFO: just a helper function
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

void test_matrix_matches_reference(void) {
  state_t *states = malloc(12288 * sizeof(state_t));
  int n = bfs_state(states);

  int *h2i = malloc((1u << 21) * sizeof(int));
  for (size_t h = 0; h < (1u << 21); h++)
    h2i[h] = -1;
  for (int i = 0; i < n; i++)
    h2i[state_hash(states[i])] = i;

  int **M = construct_transition_matrix(states, n);

  for (int i = 0; i < n; i++) {
    int row_sum = 0;
    for (int j = 0; j < n; j++)
      row_sum += M[i][j];
    TEST_ASSERT_EQUAL_INT(10, row_sum); // catches lost transitions
    for (int d = 0; d < 10; d++) {
      int target = h2i[state_hash(state_next(states[i], d))];
      TEST_ASSERT_TRUE(M[i][target] >= 1); // catches wrong-column writes
    }
  }

  for (int i = 0; i < n; i++) {
    free(M[i]);
  }
  free(M);
  free(states);
  free(h2i);
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
