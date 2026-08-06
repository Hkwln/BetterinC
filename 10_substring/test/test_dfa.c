
#ifdef TEST

#include "unity.h"

#include "dfa.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void test_is_friendly(void) {
  TEST_ASSERT_FALSE(is_friendly("9").uncovered == 0);
  TEST_ASSERT_FALSE(is_friendly("100").uncovered == 0);
  TEST_ASSERT_FALSE(is_friendly("0").uncovered == 0);
  TEST_ASSERT_FALSE(is_friendly("111111111").uncovered == 0);
  TEST_ASSERT_TRUE(is_friendly("19").uncovered == 0);
  TEST_ASSERT_TRUE(is_friendly("3523014").uncovered == 0);
  TEST_ASSERT_TRUE(is_friendly("1919").uncovered == 0);
  TEST_ASSERT_TRUE(is_friendly("195482").uncovered == 0);
}

void test_logik_works(void) {
  state_t test1 = state_new();
  printf("mask=%d, uncovered=%b\n", test1.mask, test1.uncovered);
  TEST_ASSERT_EQUAL_INT(test1.mask, 0);
  TEST_ASSERT_TRUE(test1.uncovered == 0);
  state_t next = state_next(test1, 1);
  printf("mask= %d(256 expected), uncovered=%b\n", next.mask, next.uncovered);
  TEST_ASSERT_EQUAL_INT(next.mask, 256);
  TEST_ASSERT_FALSE(next.uncovered == 0);
  next = state_next((state_t){256, 0}, 9);
  printf("mask=%d, uncovered=%b\n", next.mask, next.uncovered);
  TEST_ASSERT_EQUAL_INT(next.mask, 1);
  TEST_ASSERT_TRUE(next.uncovered == 0);
  TEST_ASSERT_TRUE(state_equal(next, (state_t){1, 0}));
}

void test_bfs_works(void) {
  // max possible distinct (mask, uncovered) states: 1024 masks x 12 uncovered
  size_t size = 12288;
  state_t *state_out = malloc(size * sizeof(state_t));
  TEST_ASSERT_NOT_NULL(state_out);
  TEST_ASSERT_EQUAL_INT(bfs_state(state_out), 11765);
  free(state_out);
}
#endif // TEST
//
