
#include "dfa.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

state_t is_friendly(const char *digits) {
  state_t state = state_new();
  for (size_t i = 0; digits[i] != '\0'; i++) {
    state = state_next(state, digits[i] - '0');
  }
  return state;
}

// initial state: empty string, nothing uncovered yet -> uncovered == 0 (z ==
// true)
state_t state_new(void) { return (state_t){0, 0, 1}; }

state_t state_next(state_t current, int digit) {
  uint16_t next_F = 0, next_C = 0;

  bool completion = (digit > 0) && (current.C & (1 << (digit - 1)));

  // update new C and F
  for (size_t r = 1; r <= 10; r++) {
    if (current.F & (1 << (r - 1)) && r > digit) {
      next_F |= (1 << ((r - digit) - 1));
    }
    if (current.C & (1 << (r - 1)) && r > digit) {
      next_C |= (1 << ((r - digit) - 1));
    }
  }

  int new_r = 10 - digit;
  next_F |= (1 << (new_r - 1));
  if (current.fresh) {
    next_C |= (1 << (new_r - 1));
  }
  // coverage update -- no time counter needed: death is "C empty", which is
  // implicit (dead states never become fresh again).
  if (completion) {
    next_C = next_F; // e jumped: every live suffix now starts before e
    return (state_t){next_F, next_C, 1};
  } else if (digit == 0 && current.fresh && current.F) {
    // a zero extends the last covering block; the empty prefix (F == 0) has
    // no covering block to extend, so "0" is not friendly
    return (state_t){next_F, next_C, 1};
  } else {
    return (state_t){next_F, next_C, 0};
  }
}

bool state_equal(state_t a, state_t b) {
  return a.F == b.F && a.C == b.C && a.fresh == b.fresh;
}

// injective over (F, C, fresh): F, C in 0..1023, fresh in 0..1 -> 0..2^21-1
uint32_t state_hash(state_t s) {
  return (uint32_t)s.F * 2048 + (uint32_t)s.C * 2 + s.fresh;
}

#define HASH_SPACE (1u << 21)

int bfs_state(state_t *states_out) {
  static uint8_t visited[HASH_SPACE >> 3]; // 2^21 hashes as a bit set
  memset(visited, 0, sizeof(visited));

  state_t queue[12288]; // safely holds all reachable states (6654)
  int head = 0, tail = 0;
  int count = 0;

  state_t start = state_new();
  uint32_t start_hash = state_hash(start);
  visited[start_hash >> 3] |= (uint8_t)(1u << (start_hash & 7));
  queue[tail++] = start;

  while (head < tail) {
    state_t cur = queue[head++];
    states_out[count++] = cur; // record reachable state

    for (int d = 0; d <= 9; d++) {
      state_t nxt = state_next(cur, d);
      uint32_t h = state_hash(nxt);
      if (!(visited[h >> 3] & (uint8_t)(1u << (h & 7)))) {
        visited[h >> 3] |= (uint8_t)(1u << (h & 7));
        queue[tail++] = nxt;
      }
    }
  }
  return count;
}
