#include "parser.h"
#include <stdio.h>
Token lookahead;
const char *cursor;

int main(void) {
  char line[256];
  line[0] = '\0';
  if (!fgets(line, sizeof(line), stdin))
    return 1;
  cursor = line;
  advance();
  stmt();
  return 0;
}
