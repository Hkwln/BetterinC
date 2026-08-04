
#include "dfa.h"
#include <stdlib.h>

state_t is_friendly(const char *digits) {
  state_t state = state_new();
  for (size_t i = 0; digits[i] != '\0'; i++) {
    state = state_next(state, digits[i] - '0');
  }
  return state;
}

state_t state_new(void) { return (state_t){0, true}; }

state_t state_next(state_t current, int digit) {

  bool completion = (digit > 0) && ((current.mask & (1 << (digit - 1))) != 0);

  state_t state_next;
  state_next.mask = 0;

  for (size_t r = 1; r <= 10; r++) {
    if (current.mask & (1 << (r - 1))) {
      if (r > digit) {
        state_next.mask |= (1 << ((r - digit) - 1));
      }
      // TODO: handle something like 195482 where after number 8 the number
      // cannot be 10 substring friendly
      // if r == digit, promise completes and is dropped
      // if r < digit, promise dies (exceeds 10) -> dropped
    }
  }
  int new_r = 10 - digit;
  state_next.mask |= (1 << (new_r - 1));
  state_next.z = completion;
  return state_next;
}

bool state_equal(state_t a, state_t b) {
  if (a.mask == b.mask) {
    if (a.z == b.z)
      return true;
  }
  return false;
}

// TODO: test state_hash
uint32_t state_hash(state_t s) { return (s.mask << 1) | s.z; }

int bfs_state(state_t *states_out) {
  // perfect hash size: mask (0..1023) shifted left 1, plus z (0/1) → 0..2047
  bool visited[2048] = {false};

  state_t queue[2048]; // safely holds all states
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

#if 1
int main(void) {
  state_t *out = malloc(2048 * sizeof(state_t));
  if (!out) {
    fprintf(stderr, "memory allocation failed\n");
  }
  printf("number of states: %d\n", bfs_state(out));
  free(out);
  return 0;
}

#endif
