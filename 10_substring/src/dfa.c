
#include "dfa.h"
#include <stdlib.h>

state_t is_friendly(const char *digits) {
  state_t state = state_new();
  for (size_t i = 0; digits[i] != '\0'; i++) {
    state = state_next(state, digits[i] - '0');
  }
  return state;
}

// initial state: empty string, nothing uncovered yet -> uncovered == 0 (z ==
// true)
state_t state_new(void) { return (state_t){0, 0}; }

state_t state_next(state_t current, int digit) {

  // bit r-1 of the mask means: some suffix has digit sum (10 - r) mod 10,
  // i.e. r more digit-sum is needed to reach a positive multiple of 10.
  // Such a promise completes exactly when the new digit equals that
  // remaining sum (r == digit) -> the string becomes friendly.
  bool completion = (digit > 0) && ((current.mask & (1 << (digit - 1))) != 0);

  state_t state_next;
  state_next.mask = 0;

  for (size_t r = 1; r <= 10; r++) {
    if (current.mask & (1 << (r - 1))) {
      if (r > digit) {
        state_next.mask |= (1 << ((r - digit) - 1));
      }
    }
  }
  int new_r = 10 - digit;
  state_next.mask |= (1 << (new_r - 1));
  // update uncovered counter: 0 == z (friendly), 11 == dead state
  if (completion) {
    state_next.uncovered = 0; // last digit completed a promise
  } else if (current.uncovered < 11) {
    state_next.uncovered = current.uncovered + 1; // one more uncovered digit
  } else {
    state_next.uncovered = 11; // capped at the dead state
  }
  return state_next;
}

bool state_equal(state_t a, state_t b) {
  if (a.mask == b.mask) {
    if (a.uncovered == b.uncovered)
      return true;
  }
  return false;
}

// TODO: test state_hash
// injective over (mask, uncovered): mask (0..1023) x uncovered (0..11) ->
// 0..12287
uint32_t state_hash(state_t s) { return (uint32_t)s.mask * 12 + s.uncovered; }

int bfs_state(state_t *states_out) {
  // perfect hash size: 1024 masks x 12 uncovered values -> 0..12287
  bool visited[12288] = {false};

  state_t queue[12288]; // safely holds all states
  int head = 0, tail = 0;
  int count = 0;

  state_t start = state_new();
  uint32_t start_hash = state_hash(start);
  visited[start_hash] = true;
  queue[tail++] = start;

  while (head < tail) {
    state_t cur = queue[head++];
    states_out[count++] = cur; // record reachable state

    for (int d = 0; d <= 9; d++) {
      state_t nxt = state_next(cur, d);
      uint32_t h = state_hash(nxt);
      if (!visited[h]) {
        visited[h] = true;
        queue[tail++] = nxt;
      }
    }
  }
  return count;
}
