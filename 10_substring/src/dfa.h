
#ifndef DFA_H
#define DFA_H
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint16_t mask; // only bits 0...9 used;
  bool z;
} state_t;

bool is_friendly(const char *digits);

state_t state_new(void); // returns the initial state
state_t
state_next(state_t current,
           int digit); // applies the transition rule  for one digit(0-9)

bool state_equal(state_t a, state_t b);

uint32_t state_hash(state_t s);

int bfs_state(state_t *states_out);

#endif // DFA_H
