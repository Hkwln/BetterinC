// this programm uses a certain amount of memory you define
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc > 0) {
    printf(" this is your argv: %s\n", argv[1]);
    long long array_size = atoi(argv[1]) * 1024 * sizeof(int);
    int *arr = malloc(array_size);
    for (;;) {
      for (int i = 0; i < array_size; i++) {
        arr[i] += 1;
      }
    }
    free(arr);
  }
  return 0;
}
