
#include "input_handling.h"
#include <errno.h>
#include <ncurses.h> //ncurses includes stdio; unctl stdarg stddef
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FLAG_SAVED (1 << 0)
#define FLAG_EXIT (1 << 1)
#define FLAG_TEMP (1 << 2)

int write_mode(FILE *file, uint8_t *perm) {
  char *buf = malloc(2000 * sizeof(char));
  int buf_len = 0;
  if ((*perm & FLAG_TEMP) == 0) {
    char c;
    for (; (c = fgetc(file)) != EOF; buf_len++) {
      addch(c);
      refresh();
      buf[buf_len] = c;
    }
  }
  buf[buf_len] = '\0';
  char *tempbuf = malloc(200 * sizeof(char));
  for (int i = 0; i < 200; i++) {
    noecho();
    tempbuf[i] = getch();
    if (delete_comb(tempbuf[i])) {
      if (i > 0) {
        i -= 2;
      } else if (buf_len > 0) {
        buf_len--;
        buf[buf_len] = '\0';
        i = -1;
      } else {
        i = -1;
        continue;
      }
      int y, x;
      getyx(stdscr, y, x);
      if (x > 0) {
        move(y, x - 1);
        delch();
        refresh();
      } else if (y > 0) {
        move(y - 1, COLS - 1);
        int prev_y, prev_x;
        getyx(stdscr, prev_y, prev_x);
        while (prev_x > 0 && inch() == ' ') {
          move(prev_y, prev_x - 1);
          getyx(stdscr, prev_y, prev_x);
        }
        if (inch() != ' ') {
          move(prev_y, prev_x + 1);
        }
        delch();
        refresh();
      }
      continue;
    }
    if (!(exit_comb(tempbuf[i]) || save_comb(tempbuf[i]))) {
      addch(tempbuf[i]);
    }
    if (exit_comb(tempbuf[i])) {
      *perm = *perm | FLAG_EXIT;
      if ((*perm & FLAG_SAVED) == 0) {
        printw("\nyou have to save the file first bevore exit\n");
      }
    }
    if (save_comb(tempbuf[i])) {
      tempbuf[i] = '\0';
      strcat(buf, tempbuf);
      fseek(file, 0, SEEK_SET);
      fwrite(buf, strlen(buf), 1, file);
      ftruncate(fileno(file), strlen(buf));
      fflush(file);
      *perm = *perm | FLAG_SAVED;
    }
    if (((*perm & FLAG_EXIT) != 0) && ((*perm & FLAG_SAVED) != 0)) {
      free(buf);
      free(tempbuf);
      return 1;
    }
  }
  free(buf);
  free(tempbuf);
  return 0;
}
bool open_editor(char *filename) {
  uint8_t perm = 0;
  // check if it is a tempfile or a opening file:
  if (filename == NULL) {
    perm |= FLAG_TEMP;
    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL) {
      printw("%s", strerror(errno));
      return false;
    }
    write_mode(file, &perm);
    addstr("\nwhat is your filename?\n");
    char *filebuf = malloc(100 * sizeof(char));
    echo();
    getnstr(filebuf, 99);
    fclose(file);
    if (rename("temp.txt", filebuf) == 0) {
      printw("successfully saved: %s \n", filebuf);
      return true;
    } else
      printw("ERROR renaming file\n");
    free(filebuf);
    refresh();
  } else if (filename != NULL) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
      printw("%s", strerror(errno));
      return true;
    }
    write_mode(file, &perm);
    fclose(file);
    return true;
  }
  return true;
}

int main(int argc, char **argv) {
  initscr();
  if (argc == 1) {
    open_editor(NULL);
  } else if (argc == 2) {
    printw("successfully opened: %s\n", argv[1]);
    refresh();
    open_editor(argv[1]);

  } else {
    printw("error you cannot edit more than one file\n");
  }
  refresh();
  endwin();
  return 0;
}
