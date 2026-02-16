
#include "input_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO: what value has a empty char? i think it is NULL
// How do i do input handling?
// Research tui handling
void write_mode(FILE *file) {
  char *buf = malloc(2000 * sizeof(char));
  char *tempbuf = malloc(200 * sizeof(char));
  scanf("%s", tempbuf);
  if (exit_comb()) {
    printf("you have to save the file first bevore exit");
  }
  if (save_comb()) {

    strcat(buf, tempbuf);
    fwrite(buf, sizeof(buf), 1, file);
  }
}
bool open_editor(char *filename) {
  // check if it is a tempfile or a opening file:
  if (filename == NULL) {
    FILE *tempfile = fopen("temp.txt", "rw");
    // TODO: build "TUI"
    write_mode(tempfile);
    if (exit_comb()) {
      printf("what is your filename?");
      scanf("%c", filename);
      // rename the temp file
    }
  } else if (filename != NULL) {
    FILE *file = fopen(filename, "rw");
    write_mode(file);
    if (exit_comb()) {
      return 0;
    }
  }
  return false;
}

int main(int argc, char **argv) {

  if (argc == 0) {
    for (; open_editor(NULL);) {
      open_editor(NULL);
    }
  } else if (argc == 1) {
    for (; open_editor(*argv);) {
      open_editor(*argv);
    }
  } else {
    printf("error you cannot edit more than one file");
  }

  return 0;
}
