#include "digitops.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
  fflush(stdout);
  printf("Welcome to your random digit game^_^\n");
  int number = rand();
  int total_digits = get_total_digits(number);
  _Bool single = 0;
  char buf[120];
  buf[0] = '\0';
  char *endptr;
  if (argc > 1) {
    printf("this is 2 player game mode :)\n");
  } else {
    printf("this is one player game mode \n");
    single = true;
  }
  printf("I created a new random number for you: \n\t\t%d\t total digits: %d\n "
         "which digit do you "
         "want to remove?\n",
         number, total_digits);
  fgets(buf, sizeof(buf), stdin);
  int digit = strtol(buf, &endptr, 10);
#if 0
  if (*endptr != '\0' || buf == endptr) {
    printf("error invalid input, you can only input numbers\n");
  }
#endif
  if (digit > total_digits) {
    digit = 0;
  }
  printf("%d\n", digit);
  number = delete_digit(number, total_digits, digit);
}
