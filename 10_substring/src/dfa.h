
#ifndef DFA_H
#define DFA_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint16_t F; // only bits 0...9 used;
  uint16_t C; // a secound bitmask, which captures old not finished promises --
              // only if both mask and C finished uncovered outputs true
  uint8_t fresh;
} state_t;

state_t is_friendly(const char *digits);

state_t state_new(void); // returns the initial state
state_t
state_next(state_t current,
           int digit); // applies the transition rule  for one digit(0-9)

bool state_equal(state_t a, state_t b);

uint32_t state_hash(state_t s);

int bfs_state(state_t *states_out);

#endif // DFA_H
