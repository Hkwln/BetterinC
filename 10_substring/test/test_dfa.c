
#ifdef TEST

#include "unity.h"

#include "dfa.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void test_is_friendly(void) {
  TEST_ASSERT_TRUE(!is_friendly("9"));
  TEST_ASSERT_TRUE(!is_friendly("100"));
  TEST_ASSERT_TRUE(!is_friendly("0"));
  TEST_ASSERT_TRUE(!is_friendly("111111111"));
  TEST_ASSERT_TRUE(is_friendly("19"));
  TEST_ASSERT_TRUE(is_friendly("3523014"));
  TEST_ASSERT_TRUE(is_friendly("1919"));
  TEST_ASSERT_TRUE("195482");
}

void test_logik_works(void) {
  state_t test1 = state_new();
  printf("mask=%d, z=%b\n", test1.mask, test1.z);
  TEST_ASSERT_EQUAL_INT(test1.mask, 0);
  TEST_ASSERT_TRUE(test1.z);
  state_t next = state_next(test1, 1);
  printf("mask= %d(256 expected), z=%b\n", next.mask, next.z);
  TEST_ASSERT_EQUAL_INT(next.mask, 256);
  TEST_ASSERT_FALSE(next.z);
  next = state_next((state_t){256, 0}, 9);
  printf("mask=%d, z=%b\n", next.mask, next.z);
  TEST_ASSERT_EQUAL_INT(next.mask, 1);
  TEST_ASSERT_TRUE(next.z);
  TEST_ASSERT_TRUE(state_equal(next, (state_t){1, true}));
}

void test_dfs_works(void) {
  state_t *state_out = malloc(500 * sizeof(state_out));
  TEST_ASSERT_EQUAL_INT(bfs_state(state_out), 474);
  free(state_out);
}
#endif // TEST
//
