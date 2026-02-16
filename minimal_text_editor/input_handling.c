#include "input_handling.h"
#include <ncurses.h>
bool exit_comb() {
  // escape == 27
  if (getch() == 27) {
    fprintf(stderr, "exit:)");
    return true;
  }
  return false;
}
bool save_comb() {
  // 1 = 49
  if (getch() == 49) {
    fprintf(stderr, "you saved it");
    return true;
  }
  return 0;
}
