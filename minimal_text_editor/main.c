
#include "input_handling.h"
#include <errno.h>
#include <ncurses.h> //ncurses includes stdio; unctl stdarg stddef
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO: implement usage of flags
int write_mode(FILE *file) {
  char *buf = malloc(2000 * sizeof(char));
  buf[0] = '\0';
  char *tempbuf = malloc(200 * sizeof(char));
  for (int i = 0; i < 200; i++) {
    tempbuf[i] = getch();
    if (exit_comb(tempbuf[i])) {
      printw("you have to save the file first bevore exit");
    }
    if (save_comb(tempbuf[i])) {
      tempbuf[i] = '\0';
      strcat(buf, tempbuf);
      fwrite(buf, strlen(buf), 1, file);
      fflush(file);
      i = -1;
    }
  }
  return 0;
}
bool open_editor(char *filename) {
  // check if it is a tempfile or a opening file:
  if (filename == NULL) {
    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL) {
      fprintf(stderr, "%s", strerror(errno));
      return false;
    }
    write_mode(file);
    addstr("what is your filename?\n");
    char *filebuf = malloc(100 * sizeof(char));
    scanf("%s", filebuf);
    fclose(file);
    if (rename("temp.txt", filebuf) == 0)
      printw("successfully saved: %s \n", filebuf);
    else
      perror("ERROR renaming file");
    free(filebuf);
    refresh();
  } else if (filename != NULL) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
      fprintf(stderr, "%s", strerror(errno));
      // load previous text:
      char c;
      while ((c = fgetc(file)) != EOF) {
        addch(c);
        refresh();
      }
      return false;
    }
    write_mode(file);
    fclose(file);
    return false;
  }
  return false;
}

int main(int argc, char **argv) {
  initscr();
  printw("%d \n", argc);
  refresh();
  if (argc == 1) {
    for (; open_editor(NULL);) {
    }
  } else if (argc == 2) {
    while (open_editor(argv[1])) {
      printw("successfully opened: %s\n", argv[1]);
      refresh();
    }
  } else {
    fprintf(stderr, "error you cannot edit more than one file\n");
  }
  refresh();
  getch();
  endwin();
  return 0;
}
