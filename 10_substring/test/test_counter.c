
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

#endif // TEST
