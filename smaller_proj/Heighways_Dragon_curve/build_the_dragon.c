
#include "build_the_dragon.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char *create_l_syst(size_t n_generations) {
  size_t total_len = (1ULL << (n_generations + 1)) - 1;
  char *start = calloc(total_len + 1, sizeof(char));
  char *tmp = calloc(total_len + 1, sizeof(char));
  strcpy(start, "f-h");
  size_t len = 0;
  for (size_t i = 1; i < n_generations; i++) {
    tmp[0] = '\0';
    // iterate start and replace all f with rule one and h with rule 2
    for (size_t b = 0; b < strlen(start); b++) {
      switch (start[b]) {
      case 'f':
        strcat(tmp, "f-h");
        break;
      case 'h':
        strcat(tmp, "f+h");
        break;
      default:
        len = strlen(tmp);
        tmp[len++] = start[b];
        tmp[len] = '\0';
      }
    }
    strcpy(start, tmp);
  }
  free(tmp);
  return start;
}

bool test_create_l_system(void) {

  char *start = create_l_syst(2);
  bool res = true;
  if ((strcmp("f-h-f+h", start) != 0)) {
    res = false;
  }
  free(start);
  start = create_l_syst(3);
  if ((strcmp("f-h-f+h-f-h+f+h", start) != 0)) {
    res = false;
  }
  free(start);
  start = create_l_syst(4);
  if ((strcmp("f-h-f+h-f-h+f+h-f-h-f+h+f-h+f+h", start) != 0)) {
    res = false;
  }
  free(start);
  return res;
}
#if 0
int main(void) {

  test_create_l_system() ? printf("all test passed\n")
                         : fprintf(stderr, "test did not pass\n");
}
#endif
