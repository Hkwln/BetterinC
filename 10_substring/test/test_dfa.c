
#ifdef TEST

#include "unity.h"

#include "dfa.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void test_is_friendly(void) {
  TEST_ASSERT_FALSE(is_friendly("9").fresh == 1);
  TEST_ASSERT_FALSE(is_friendly("100").fresh == 1);
  TEST_ASSERT_FALSE(is_friendly("0").fresh == 1);
  TEST_ASSERT_FALSE(is_friendly("111111111").fresh == 1);
  TEST_ASSERT_FALSE(is_friendly("19819").fresh == 1);
  TEST_ASSERT_TRUE(is_friendly("19").fresh == 1);
  TEST_ASSERT_TRUE(is_friendly("3523014").fresh == 1);
  TEST_ASSERT_TRUE(is_friendly("1919").fresh == 1);
  TEST_ASSERT_FALSE(is_friendly("195482").fresh == 1);
}

void test_logik_works(void) {
  state_t test1 = state_new();
  TEST_ASSERT_EQUAL_INT(test1.F, 0);
  TEST_ASSERT_EQUAL_INT(test1.C, 0);
  TEST_ASSERT_TRUE(test1.fresh);

  // "1": promise r=9 (bit 8 = 256) in both F and C; nothing covered yet
  state_t next = state_next(test1, 1);
  TEST_ASSERT_EQUAL_INT(next.F, 256);
  TEST_ASSERT_EQUAL_INT(next.C, 256);
  TEST_ASSERT_FALSE(next.fresh);

  // "9" completes the r=9 promise -> "19" fully covered
  next = state_next((state_t){256, 256, 0}, 9);
  TEST_ASSERT_EQUAL_INT(next.F, 1);
  TEST_ASSERT_EQUAL_INT(next.C, 1);
  TEST_ASSERT_TRUE(next.fresh);
  TEST_ASSERT_TRUE(state_equal(next, (state_t){1, 1, 1}));
}

void test_bfs_works(void) {
  // the new (F, C, fresh) automaton has 6654 reachable states
  size_t size = 12288;
  state_t *state_out = malloc(size * sizeof(state_t));
  TEST_ASSERT_NOT_NULL(state_out);
  int num = bfs_state(state_out);
  TEST_ASSERT_EQUAL_INT(num, 6654);
  free(state_out);
}
#endif // TEST
//
