#include <stdio.h>
#include <stdlib.h>
// just a smal test to see where the memory is located:
int main(void) {
  printf("location of main function: %p\n", main);
  printf("the heap is located here: %p\n", malloc(2060));
  int a[5];
  printf("and the stack is here: %p\n", &a);
  return 0;
}
