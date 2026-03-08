#include "digitops.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
  printf("Welcome to your random digit game^_^\n");
  int number = rand();
  int total_digits = get_total_digits(number);
  _Bool single = 0;
  char buf[120];
  int count = 0;
  buf[0] = '\0';
  char *endptr;
  if (argc > 1) {
    printf("this is 2 player game mode :)\n");
  } else {
    printf("this is one player game mode \n");
    single = true;
  }
  printf("I created a new random number for you: %d\n\t\t\t total digits: %d\n"
         "which digit do you "
         "want to remove?\n",
         number, total_digits);
  for (; number; count++) {
    fgets(buf, sizeof(buf), stdin);
    int digit = strtol(buf, &endptr, 10);
    if (digit > total_digits) {
      digit = 0;
    }
    number = delete_digit(number, total_digits, digit);
    printf("new number: %d\n", number);
  }
  if (count % 2 == 0)
    printf("player 1 won \ncongrats!!\n");
  else
    printf("player 2 won");
}
