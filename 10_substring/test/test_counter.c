
#ifdef TEST

#include "counter.h"
#include "dfa.h"
#include "matrix.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

void test_count_length(void) {

  TEST_ASSERT_EQUAL_UINT(0, (unsigned int)count_length(1));
  TEST_ASSERT_EQUAL_UINT(9, (unsigned int)count_length(2));
}

/* values verified against a brute-force enumeration of all numbers of each
 * length (see build/final_main.c): 0, 9, 63, 435, 2985, 20205, 135243 */
void test_count_length_more(void) {
  TEST_ASSERT_EQUAL_UINT(63, (unsigned int)count_length(3));
  TEST_ASSERT_EQUAL_UINT(435, (unsigned int)count_length(4));
  TEST_ASSERT_EQUAL_UINT(2985, (unsigned int)count_length(5));
  TEST_ASSERT_EQUAL_UINT(20205, (unsigned int)count_length(6));
  TEST_ASSERT_EQUAL_UINT(135243, (unsigned int)count_length(7));
}

void test_count_up_to(void) {
  TEST_ASSERT_EQUAL_UINT64(0ULL, count_up_to(0));
  TEST_ASSERT_EQUAL_UINT64(9ULL, count_up_to(2));
  TEST_ASSERT_EQUAL_UINT64(72ULL, count_up_to(3));
  TEST_ASSERT_EQUAL_UINT64(507ULL, count_up_to(4));
  TEST_ASSERT_EQUAL_UINT64(170786910011778ULL, count_up_to(18));
}

#endif // TEST
