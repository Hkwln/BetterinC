
#include "input_handling.h"
#include <ncurses.h> //ncurses includes stdio; unctl stdarg stddef
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void write_mode(FILE *file) {
  char *buf = malloc(2000 * sizeof(char));
  char *tempbuf = malloc(200 * sizeof(char));
  for (int i = 0; i < sizeof(*tempbuf); i++) {
    tempbuf[i] += getch();
    if (exit_comb()) {
      printw("you have to save the file first bevore exit");
    }
    if (save_comb()) {
      tempbuf[i + 1] = '\0';
      strcat(buf, tempbuf);
      fwrite(buf, sizeof(buf), 1, file);
    }
  }
}
bool open_editor(char *filename) {
  // check if it is a tempfile or a opening file:
  if (filename == NULL) {
    FILE *tempfile = fopen("temp.txt", "rw");
    // TODO: build "TUI"
    write_mode(tempfile);
    if (exit_comb()) {
      addstr("what is your filename?");
      scanf("%c", filename);
      // rename the temp file
    }
  } else if (filename != NULL) {
    FILE *file = fopen(filename, "rw");
    write_mode(file);
    if (exit_comb()) {
      return false;
    }
  }
  return false;
}

int main(int argc, char **argv) {
  initscr();
  printw("%d \n", argc);
  refresh();
  if (argc == 1) {
    for (; open_editor(NULL);) {
      open_editor(NULL);
    }
  } else if (argc == 2) {
    while (open_editor(*argv)) {
      open_editor(*argv);
    }
  } else {
    fprintf(stderr, "error you cannot edit more than one file");
  }
  refresh();
  endwin();
  return 0;
}
