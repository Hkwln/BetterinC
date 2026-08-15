#ifndef RECUR_H
#define RECUR_H

#include <stddef.h>

/*
 * Berlekamp-Massey + Kitamasa over GF(1000000007).
 *
 * berlekamp_massey() fits the shortest linear recurrence of a sequence:
 *   s[i] + C[1]*s[i-1] + ... + C[L]*s[i-L] == 0  (mod 1e9+7)  for all i >= L
 * result->C is the connection polynomial with C[0] = 1 and result->L its
 * degree (the minimal order).  result->C is heap-allocated; the caller frees
 * it.
 *
 * kitamasa() evaluates the n-th term of a recurrence given as
 *   s[n] = rec[0]*s[n-1] + rec[1]*s[n-2] + ... + rec[d-1]*s[n-d]
 * with the first d terms init[0..d-1].  Bridge from BM:
 *   rec[j-1] = (MOD - C[j]) % MOD,  for j = 1..L
 */
struct BMResult {
  long long *C; /* connection polynomial, C[0] == 1, caller frees */
  int L;        /* minimal order */
};

void berlekamp_massey(const int *sequence, size_t length,
                      struct BMResult *result);

long long kitamasa(const long long *rec, int d, const long long *init,
                   long long n);

#endif // RECUR_H
