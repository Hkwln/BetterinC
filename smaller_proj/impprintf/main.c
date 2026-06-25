#include "customformatter.h"

int main(void) {
  char buf[3000];
  // test:
  snformat(buf, sizeof(buf), "value: %d hex: %x name: %s ;%c\n", 42, 255,
           "Alice", "a");
  format("%s", buf);
  return 0;
}
