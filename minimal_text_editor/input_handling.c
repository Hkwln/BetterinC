#include "input_handling.h"
#include <ncurses.h>
bool exit_comb(int i) {
  // escape == 27
  if ((i) == 27) {
    return true;
  }
  return false;
}
bool save_comb(int i) {
  // 1 = 49
  if ((i) == 49) {
    return true;
  }
  return 0;
}
bool delete_comb(int i) {
  if ((i) == 263 || i == KEY_BACKSPACE || i == 127 || i == 8) {
    return true;
  }
  return 0;
}
