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
  int j = 0;
  for (int i = 0; number; i++) {
    if (i == digit - 1) {
      number /= 10;
      deleted++;
      continue;
    }
    tmparr[j++] = number % 10;
    number /= 10;
  }
  for (int builder = j - 1; builder >= 0; builder--) {
    new_number = (new_number * 10) + tmparr[builder];
  }
  return new_number;
}
#if 0
// test
int main() {
  printf("expected 15: %d\n", delete_digit(1035, 4, 4));

  return 1;
}
#endif
