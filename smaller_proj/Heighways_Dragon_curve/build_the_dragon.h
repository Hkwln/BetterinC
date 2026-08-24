#pragma once
#include <stddef.h>
#include <stdio.h>

char *create_l_syst(size_t n_generations);

// --- O(1)-memory streaming accessors -------------------------------------
// The order-n L-system string is 2^(n+1)-1 chars, so for large n it can
// never be held in memory.  Every symbol, however, is a pure function of its
// position, so it can be computed on the fly:
//   * even positions  alternate 'f','h','f','h',...
//   * odd position 2i+1 is the i-th turn, given by the paperfolding
//     sequence: '+' iff the odd part of (i+1) is 3 (mod 4), else '-'.
// No state is needed beyond the position itself.

// Turn ('-' or '+') taken after segment i of the dragon curve (i >= 0).
char dragon_turn_at(size_t i);

// Symbol at position pos of the order-n_generations L-system string,
// or '\0' if pos is out of range (n_generations must be < 63).
char dragon_symbol_at(size_t pos, size_t n_generations);
