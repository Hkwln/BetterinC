#include <math.h>
#include <stdio.h>
int get_total_digits(int starting_number) {
  int count = 0;
  while (starting_number) {
    starting_number /= 10;
    count++;
  }
  return count;
}
// must be a valid digit
int delete_digit(int number, int total_digits, int digit) {
  int tmparr[total_digits];
  int deleted = 0;
  int new_number = 0;
  if (total_digits == digit) {
    // TODO: further checks:
  }
  for (int i = 0; number; i++) {
    if (i == digit - 1) {
      number /= 10;
      deleted++;
      continue;
    }
    tmparr[i] = number % 10;
    number /= 10;
  }
  // FIXME: build the a number out of the array:
  for (int builder = total_digits - deleted; builder >= 0; builder--) {
    new_number = tmparr[builder] * pow((total_digits - deleted), 10);
  }
  return new_number;
}
#if 1
// test
int main() {
  printf("expected 15: %d\n", delete_digit(135, 3, 2));

  return 1;
}
#endif
