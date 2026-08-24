
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
        strcat(tmp, "f+h");
        break;
      case 'h':
        strcat(tmp, "f-h");
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

char dragon_turn_at(size_t i) {
  size_t odd = i + 1;
  while ((odd & 1u) == 0)
    odd >>= 1; // strip the factors of two -> odd part of (i+1)
  return ((odd & 3u) == 3u) ? '+' : '-';
}

char dragon_symbol_at(size_t pos, size_t n_generations) {
  if (n_generations >= 63 || pos >= ((1ULL << (n_generations + 1)) - 1))
    return '\0';
  if ((pos & 1u) == 0) // forward symbol: 'f','h' alternate
    return (((pos >> 1) & 1u) == 0) ? 'f' : 'h';
  return dragon_turn_at((pos - 1) / 2);
}

bool test_streaming_matches_string(void) {
  for (size_t n = 2; n <= 12; n++) {
    char *s = create_l_syst(n);
    for (size_t pos = 0; s[pos]; pos++) {
      if (dragon_symbol_at(pos, n) != s[pos]) {
        free(s);
        fprintf(stderr, "streaming mismatch at n=%zu pos=%zu\n", n, pos);
        return false;
      }
    }
    free(s);
  }
  return true;
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
#ifdef DRAGON_TEST
int main(void) {
  bool ok = test_create_l_system() && test_streaming_matches_string();
  if (ok)
    printf("all tests passed\n");
  else
    fprintf(stderr, "test did not pass\n");
  return ok ? 0 : 1;
}
#endif
