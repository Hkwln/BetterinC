#include <errno.h>
#include <stdio.h>
#include <string.h>
typedef struct {
  size_t lines;
  size_t chars;
  size_t numberoffiles;
} fileinfo;
// TODO: add error handling
int main(int argc, char **argv) {
  // initialize fileinfo:
  fileinfo info;
  info.lines = 0;
  info.chars = 0;
  info.numberoffiles = 0;
  for (int i = 1; i < argc; i++) {
    char *filename = argv[i];
    // check of file actually exist
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
      fprintf(stderr, "no such file or directory");
      return ENOENT;
      continue; // skip to next loop
    }
    info.numberoffiles++;
    char mystring[100];
    while (fgets(mystring, 100, file)) {
      info.lines++;
      for (int b = 0; b < strlen(mystring); b++) {
        if (mystring[b] != ' ' && mystring[b] != '\n' && mystring[b] != '\0')
          info.chars++;
      }
    }
    // count which chars are full and which are not

    fclose(file);
  }
  fprintf(stdout, "number of chars: %zu\n", info.chars);
  printf("number of lines: %zu \n", info.lines);
  printf("number of files %zu\n", info.numberoffiles);
  return 1;
}
