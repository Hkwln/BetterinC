#include <stdbool.h>
#include <stdio.h>
// convert from int to char
int main(void) {

  int num = -60;
  bool isNEG = num < 0;

  unsigned int n1 = isNEG ? -num : num;
  char number[50];
  if (n1 == 0) {
    number[0] = '0';
    number[1] = '\0';
  }

  int i = 0;
  for (; n1 != 0;) {
    number[i++] = (n1 % 10) + '0';
    n1 /= 10;
  }
  if (isNEG)
    number[i++] = '-';

  number[i] = '\0';
  // XXX: what is happening here?
  for (int t = 0; t < i / 2; t++) {
    number[t] ^= number[i - t - 1];
    number[i - t - 1] ^= number[t];
    number[t] ^= number[i - t - 1];
  }
  printf("%s\n", number);
}
