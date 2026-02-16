#include "input_handling.h"
#include <ncurses.h>
bool exit_comb(int i) {
  // escape == 27
  if ((i) == 27) {
    fprintf(stderr, "exit:)");
    return true;
  }
  return false;
}
bool save_comb(int i) {
  // 1 = 49
  if ((i) == 49) {
    fprintf(stderr, "you saved it");
    return true;
  }
  return 0;
}
