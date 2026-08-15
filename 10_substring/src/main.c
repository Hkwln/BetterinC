// T(10^18) mod 1 000 000 007
//
// T(N) = number of friendly numbers (strings of digits 1..9, first digit
// nonzero) with at most N digits.  The DFA in dfa.c recognizes friendly
// strings; count_length(L) is a C-finite sequence, so the prefix sums
// P(L) = T(L) satisfy a linear recurrence of order 2772 (found by
// Berlekamp-Massey), and Kitamasa evaluates it at n = 10^18.
//
// Pipeline:
//   1. bfs_state()  -> all reachable DFA states
//   2. vector iteration over the state graph -> P(1)..P(20000) (mod MOD)
//   3. berlekamp_massey(P) -> minimal recurrence (order 2772)
//   4. kitamasa(rec, 2772, P, 10^18) -> the answer
//
// NOTE on the sequence: Berlekamp-Massey does NOT consume the state masks
// (F, C, fresh).  Those describe the automaton; the numeric sequence it
// needs is the *count* of friendly strings of each length (here stored as
// prefix sums P(L), because the problem asks for the total up to 10^18).

#include "dfa.h"
#include "recur.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 1000000007LL
#define NTERMS 20000 /* >= 2 * minimal order (2772) with margin */
#define MAXSTATES 12288

int main(void) {
  /* ---- 1. enumerate all reachable DFA states ------------------------ */
  state_t *states = malloc(MAXSTATES * sizeof(state_t));
  int N = bfs_state(states);

  int *h2i = malloc(((size_t)1 << 21) * sizeof(int));
  for (size_t h = 0; h < (1u << 21); h++)
    h2i[h] = -1;
  for (int i = 0; i < N; i++)
    h2i[state_hash(states[i])] = i;

  /* ---- 2a. precompute the transition table (N x 10) ------------------ */
  int (*trans)[10] = malloc((size_t)N * 10 * sizeof(int));
  for (int u = 0; u < N; u++)
    for (int d = 0; d < 10; d++)
      trans[u][d] = h2i[state_hash(state_next(states[u], d))];

  /* ---- 2b. count friendly strings of each length, accumulate P(L) ----
   * v[u] = number of length-L strings ending in state u (mod MOD).
   * First digit 1..9 (no leading zeros), all later digits 0..9.          */
  static long long v[MAXSTATES], nv[MAXSTATES], P[NTERMS + 1];
  for (int d = 1; d <= 9; d++)
    v[trans[0][d]] = (v[trans[0][d]] + 1) % MOD;

  P[0] = 0;
  long long acc = 0;
  for (int L = 1; L <= NTERMS; L++) {
    for (int k = 0; k < N; k++) /* states with fresh == 1 are accepting */
      if (states[k].fresh == 1)
        acc = (acc + v[k]) % MOD;
    P[L] = acc; /* P(L) = T(L) = total friendly numbers with <= L digits */

    memset(nv, 0, (size_t)N * sizeof(long long));
    for (int u = 0; u < N; u++) {
      if (!v[u])
        continue;
      for (int d = 0; d < 10; d++)
        nv[trans[u][d]] = (nv[trans[u][d]] + v[u]) % MOD;
    }
    memcpy(v, nv, (size_t)N * sizeof(long long));
  }
  free(h2i);

  printf("states = %d\n", N);
  printf("P(2)   = %lld (expect 9)\n", P[2]);
  printf("P(5)   = %lld (expect 3492)\n", P[5]);
  printf("P(10)  = %lld (expect 46402069)\n", P[10]);

  /* ---- 3. Berlekamp-Massey on the prefix sums ------------------------ */
  int *seq = malloc((NTERMS + 1) * sizeof(int));
  for (int i = 0; i <= NTERMS; i++)
    seq[i] = (int)P[i]; /* P[i] < MOD fits in int */
  struct BMResult res;
  berlekamp_massey(seq, NTERMS + 1, &res);
  free(seq);
  printf("recurrence order L = %d (expect 2772)\n", res.L);

  /* ---- 4. bridge: connection polynomial C -> recurrence rec ---------- */
  long long *rec = malloc((size_t)(res.L + 1) * sizeof(long long));
  for (int j = 1; j <= res.L; j++)
    rec[j - 1] = (MOD - res.C[j]) % MOD; /* s[n] = sum rec[j-1] s[n-j] */
  free(res.C);

  /* ---- 5. Kitamasa: the killer check, then the answer ---------------- */
  long long check = kitamasa(rec, res.L, P, 20000);
  printf("kitamasa(20000) = %lld (expect P(20000) = %lld)\n", check, P[20000]);

  long long answer = kitamasa(rec, res.L, P, 1000000000000000000LL); /* 10^18 */
  printf("T(10^18) mod 1e9+7 = %lld (expect 23624465)\n", answer);

  free(rec);
  free(trans);
  free(states);
  return 0;
}
