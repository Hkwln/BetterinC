#include "recur.h"

#include <stdlib.h>
#include <string.h>

#define MOD 1000000007LL

/* modular exponentiation; Fermat's little theorem gives the inverse of b as
 * b^(MOD-2) since MOD is prime */
static long long modpow(long long b, long long e) {
  long long r = 1;
  b %= MOD;
  while (e > 0) {
    if (e & 1)
      r = r * b % MOD;
    b = b * b % MOD;
    e >>= 1;
  }
  return r;
}

/* Berlekamp-Massey over GF(MOD): fills result->C (C[0] = 1) with
 *   s[i] + C[1]*s[i-1] + ... + C[L]*s[i-L] == 0  (mod MOD)   for i >= L
 * and result->L = L, the minimal order.  result->C is heap-allocated. */
void berlekamp_massey(const int *sequence, size_t length,
                      struct BMResult *result) {
  long long *C = calloc(length + 1, sizeof(long long));
  long long *B = calloc(length + 1, sizeof(long long));
  long long *T = malloc((length + 1) * sizeof(long long));
  C[0] = B[0] = 1;

  int L = 0;    /* current order */
  int m = 1;    /* shift since the last adjustment */
  long long b = 1; /* the discrepancy of the last adjustment */

  for (size_t i = 0; i < length; i++) {
    /* discrepancy: d = s[i] + C[1]*s[i-1] + ... + C[L]*s[i-L] */
    long long d = sequence[i] % MOD;
    for (int j = 1; j <= L; j++)
      d = (d + C[j] * sequence[i - j]) % MOD;
    d = (d % MOD + MOD) % MOD;

    if (d == 0) { /* sequence matches the current recurrence */
      m++;
      continue;
    }

    memcpy(T, C, (length + 1) * sizeof(long long)); /* remember old C */

    /* C = C - (d/b) * x^m * B  (mod MOD) */
    long long coef = d * modpow(b, MOD - 2) % MOD;
    for (int j = m; j < (int)length; j++)
      C[j] = ((C[j] - coef * B[j - m]) % MOD + MOD) % MOD;

    if (2 * L > (int)i) { /* recurrence not long enough to improve */
      m++;
      continue;
    }
    L = i + 1 - L; /* longer recurrence needed */
    memcpy(B, T, (length + 1) * sizeof(long long));
    b = d;
    m = 1;
  }

  result->C = malloc((size_t)(L + 1) * sizeof(long long));
  for (int j = 0; j <= L; j++)
    result->C[j] = C[j];
  result->L = L;

  free(C);
  free(B);
  free(T);
}

/* multiply a,b (degree < d) and reduce mod the characteristic polynomial
 *   x^d == rec[0]*x^(d-1) + rec[1]*x^(d-2) + ... + rec[d-1]
 * i.e. every x^i with i >= d folds to sum_j rec[j-1] * x^(i-j).
 * scratch must hold at least 2*d-1 entries; out receives degree < d. */
static void poly_mul(const long long *a, const long long *b,
                     const long long *rec, int d, long long *scratch,
                     long long *out) {
  memset(scratch, 0, (size_t)(2 * d - 1) * sizeof(long long));

  /* plain convolution */
  for (int i = 0; i < d; i++)
    if (a[i])
      for (int j = 0; j < d; j++)
        if (b[j])
          scratch[i + j] = (scratch[i + j] + a[i] * b[j]) % MOD;

  /* reduction, high degree first (folding creates only lower degrees) */
  for (int i = 2 * d - 2; i >= d; i--) {
    long long c = scratch[i];
    if (!c)
      continue;
    for (int j = 1; j <= d; j++)
      scratch[i - j] = (scratch[i - j] + c * rec[j - 1]) % MOD;
  }

  memcpy(out, scratch, (size_t)d * sizeof(long long));
}

/* s[n] = rec[0]*s[n-1] + ... + rec[d-1]*s[n-d], init = s[0..d-1].
 * Computes x^n mod the characteristic polynomial and contracts with init:
 * s[n] = sum_i [x^i] x^n * s[i]. */
long long kitamasa(const long long *rec, int d, const long long *init,
                   long long n) {
  if (d <= 0)
    return 0;
  if (n < d) /* x^n has degree < d: no reduction, answer is init[n] */
    return init[n] % MOD;

  long long *res = calloc((size_t)(2 * d - 1), sizeof(long long));
  long long *base = calloc((size_t)(2 * d - 1), sizeof(long long));
  long long *scratch = malloc((size_t)(2 * d - 1) * sizeof(long long));
  res[0] = 1;  /* polynomial 1 */
  base[1] = 1; /* polynomial x */

  long long e = n;
  while (e > 0) {
    if (e & 1)
      poly_mul(res, base, rec, d, scratch, res);
    poly_mul(base, base, rec, d, scratch, base);
    e >>= 1;
  }

  long long ans = 0;
  for (int i = 0; i < d; i++)
    ans = (ans + res[i] * init[i]) % MOD;

  free(res);
  free(base);
  free(scratch);
  return ans;
}
