#include <ncurses.h>

int main() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  int ch;
  while ((ch = getch()) != 27) {
    printw("key pressed:%d\n", ch);
    refresh();
  }
  endwin();
  return 0;
}
