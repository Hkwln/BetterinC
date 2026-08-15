
#include "counter.h"
#include "dfa.h"
#include "stdio.h"
#include "stdlib.h"

#include <string.h>

unsigned long count_length(size_t L) {
  state_t *pos_stat = calloc(12288, sizeof(state_t));
  size_t N = bfs_state(pos_stat);
  int *hash_to_index = malloc((1u << 21) * sizeof(int));
  for (size_t b = 0; b < (1u << 21); b++) {
    hash_to_index[b] = -1;
  }
  for (size_t b = 0; b < N; b++) {
    hash_to_index[state_hash(pos_stat[b])] = (int)b;
  }

  unsigned long v1[12288]; // initialize
  memset(v1, 0, sizeof(v1));
  int t = 0;
  for (size_t d = 1; d < 10; d++) {
    t = hash_to_index[state_hash(state_next(pos_stat[0], d))];
    v1[t] += 1;
  }
  unsigned long v[12288];
  memset(v, 0, sizeof(v));
  unsigned long *new_v = calloc(N, sizeof(unsigned long));

  size_t count = 0;
  memcpy(v, v1, N * sizeof(unsigned long));
  for (size_t u = 0; u < N; u++) {
    for (size_t d = 0; d < 10; d++) {
      t = hash_to_index[state_hash(state_next(pos_stat[u], d))];
      new_v[t] += v[u];
    }
  }
  count++;
  while (count < L) {
    memcpy(v, new_v, N * sizeof(unsigned long));
    memset(new_v, 0, N * sizeof(unsigned long));
    // reset new_v
    for (size_t u = 0; u < N; u++) {
      for (size_t d = 0; d < 10; d++) {
        t = hash_to_index[state_hash(state_next(pos_stat[u], d))];
        new_v[t] += v[u];
      }
    }
    count++;
  }

  unsigned long result = 0;
  for (size_t k = 0; k < N; k++) {
    if (pos_stat[k].fresh == 1) {
      result += v[k];
    }
  }
  free(pos_stat);
  free(new_v);
  free(hash_to_index);
  return result;
}

/* basically the same as  count_length, but instead of only calculating L
 * directly it calculates all previous up to L*/

unsigned long long count_up_to(size_t L) {
  state_t *out = calloc(12288, sizeof(state_t));
  size_t N = bfs_state(out);
  int *hash_to_index = malloc((1u << 21) * sizeof(int));
  for (size_t b = 0; b < (1u << 21); b++) {
    hash_to_index[b] = -1;
  }
  for (size_t b = 0; b < N; b++) {
    hash_to_index[state_hash(out[b])] = (int)b;
  }
  unsigned long v1[12288];
  memset(v1, 0, sizeof(v1));
  unsigned long v[12288];
  memset(v, 0, sizeof(v));
  unsigned long new_v[12288];
  memset(new_v, 0, sizeof(new_v));
  unsigned long t = 0;

  for (size_t i = 1; i < 10; i++) {
    t = hash_to_index[state_hash(state_next(out[0], i))];
    v1[t] += 1;
  }
  unsigned long long result = 0;
  memcpy(v, v1, N * sizeof(unsigned long));
  for (size_t i = 1; i <= L; i++) {
    // count_length(i) = accepting sum of v
    for (size_t k = 0; k < N; k++) {
      if (out[k].fresh == 1) {
        result += v[k];
      }
    }
    // advance one digit for the next length
    if (i < L) {
      memset(new_v, 0, N * sizeof(unsigned long));
      for (size_t u = 0; u < N; u++) {
        for (size_t d = 0; d < 10; d++) {
          int t = hash_to_index[state_hash(state_next(out[u], d))];
          new_v[t] += v[u];
        }
      }
      memcpy(v, new_v, N * sizeof(unsigned long));
    }
  }
  free(out);
  free(hash_to_index);
  return result;
}
