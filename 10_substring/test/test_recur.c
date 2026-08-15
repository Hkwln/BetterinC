#ifdef TEST

#include "unity.h"

#include "recur.h"
#include <stdlib.h>

#define MOD 1000000007LL

void setUp(void) {}

void tearDown(void) {}

/* residual property: the returned C must reproduce every term:
   s[i] + C[1]*s[i-1] + ... + C[L]*s[i-L] == 0 (mod MOD)  for all i >= L */
static int bm_holds(const int *s, size_t n, const long long *C, size_t L) {
  for (size_t i = L; i < n; i++) {
    long long acc = s[i];
    for (size_t j = 1; j <= L; j++)
      acc = ((acc + C[j] * s[i - j]) % MOD + MOD) % MOD;
    if (acc != 0)
      return 0;
  }
  return 1;
}

/* contract: the function allocates result->C (caller frees it) */
static void check_bm(const int *s, size_t n, size_t wantL,
                     const long long *wantC) {
  struct BMResult res;
  berlekamp_massey(s, n, &res);

  TEST_ASSERT_NOT_NULL(res.C);
  TEST_ASSERT_EQUAL_UINT(wantL, res.L);                   // exact minimal order
  TEST_ASSERT_EQUAL_INT64(1, res.C[0]);                   // C[0] invariant
  TEST_ASSERT_TRUE(bm_holds(s, n, res.C, (size_t)res.L)); // reproduces data
  for (size_t j = 1; j <= wantL; j++)
    TEST_ASSERT_EQUAL_INT64(wantC[j - 1], res.C[j]); // exact coefficients

  free(res.C);
}

void test_bm_zero_sequence(void) {
  int s[] = {0, 0, 0, 0, 0};
  check_bm(s, 5, 0, NULL); // zero sequence: order 0
}

void test_bm_constant(void) {
  int s[] = {7, 7, 7, 7, 7};
  long long c[] = {MOD - 1}; // s[n] = s[n-1]
  check_bm(s, 5, 1, c);
}

void test_bm_geometric(void) {
  int s[] = {3, 9, 27, 81, 243};
  long long c[] = {MOD - 3}; // s[n] = 3*s[n-1]
  check_bm(s, 5, 1, c);
}

void test_bm_arithmetic(void) {
  int s[] = {1, 3, 5, 7, 9, 11, 13};
  long long c[] = {MOD - 2, 1}; // s[n] = 2*s[n-1] - s[n-2]
  check_bm(s, 7, 2, c);
}

void test_bm_fibonacci(void) {
  int s[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
  long long c[] = {MOD - 1, MOD - 1}; // s[n] = s[n-1] + s[n-2]
  check_bm(s, 10, 2, c);
}

void test_bm_order3(void) {
  int s[25];
  s[0] = 1;
  s[1] = 2;
  s[2] = 4;
  for (int i = 3; i < 25; i++) // s[n] = 2*s[n-1] + 3*s[n-2] + 5*s[n-3]
    s[i] = (int)((2LL * s[i - 1] + 3LL * s[i - 2] + 5LL * s[i - 3]) % MOD);
  long long c[] = {MOD - 2, MOD - 3, MOD - 5};
  check_bm(s, 25, 3, c);
}

/* ---- Kitamasa ------------------------------------------------------ */

/* checkpoints independently computed by naive iteration (see kitamasa.md) */
void test_kitamasa_fibonacci(void) {
  long long rec[] = {1, 1};    /* s[n] = s[n-1] + s[n-2] */
  long long init[] = {0, 1};   /* s[0] = 0, s[1] = 1 */
  long long n[] = {5, 10, 20, 30, 50, 100, 1000};
  long long want[] = {5, 55, 6765, 832040, 586268941, 687995182, 517691607};
  for (size_t i = 0; i < 7; i++)
    TEST_ASSERT_EQUAL_INT64(want[i], kitamasa(rec, 2, init, n[i]));
}

void test_kitamasa_order3(void) {
  long long rec[] = {2, 3, 5};  /* s[n] = 2*s[n-1] + 3*s[n-2] + 5*s[n-3] */
  long long init[] = {1, 2, 4};
  long long n[] = {10, 20, 30, 100};
  long long want[] = {83359, 582864947, 547077970, 485739975};
  for (size_t i = 0; i < 4; i++)
    TEST_ASSERT_EQUAL_INT64(want[i], kitamasa(rec, 3, init, n[i]));
}

/* n < d must work without exponentiation: x^n already has degree < d */
void test_kitamasa_small_n(void) {
  long long rec[] = {1, 1};
  long long init[] = {0, 1};
  TEST_ASSERT_EQUAL_INT64(0, kitamasa(rec, 2, init, 0));
  TEST_ASSERT_EQUAL_INT64(1, kitamasa(rec, 2, init, 1));
  TEST_ASSERT_EQUAL_INT64(1, kitamasa(rec, 2, init, 2)); /* fib(2) */
  TEST_ASSERT_EQUAL_INT64(3, kitamasa(rec, 2, init, 4)); /* fib(4) */
}

/* d == 0 (zero sequence): degenerate, must not crash */
void test_kitamasa_order_zero(void) {
  long long init[] = {0};
  TEST_ASSERT_EQUAL_INT64(0, kitamasa(NULL, 0, init, 1000000000000000000LL));
}

/* bridge: BM's connection polynomial C -> recurrence rec -> kitamasa */
void test_bm_to_kitamasa_bridge(void) {
  int s[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610};
  struct BMResult res;
  berlekamp_massey(s, 16, &res);
  TEST_ASSERT_EQUAL_INT(2, res.L);
  long long rec[2];
  rec[0] = (MOD - res.C[1]) % MOD;
  rec[1] = (MOD - res.C[2]) % MOD;
  free(res.C);
  long long init[] = {0, 1};
  TEST_ASSERT_EQUAL_INT64(1, rec[0]);
  TEST_ASSERT_EQUAL_INT64(1, rec[1]);
  TEST_ASSERT_EQUAL_INT64(987, kitamasa(rec, 2, init, 16));   /* fib(16) */
  TEST_ASSERT_EQUAL_INT64(517691607, kitamasa(rec, 2, init, 1000));
}

#endif // TEST
