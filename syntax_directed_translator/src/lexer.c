#include "lexer.h"
#include <stdio.h>
/*
Implementiere `Token next_token(const char **src)` — sie liest das nächste Token
aus `*src` und rückt den Zeiger vor. Leerzeichen überspringen.
 * */
Token next_token(const char **src) {
  Token token;
  printf("%d\n", **src);
  char c = **src;
  (*src)++;
  if (c == '+') {
    token.type = TOKEN_PLUS;
  } else if (c == '-') {
    token.type = TOKEN_MINUS;
  } else if (c == '*') {
    token.type = TOKEN_STAR;
  } else if (c == '/') {
    token.type = TOKEN_SLASH;
  } else if (c == '(') {
    token.type = TOKEN_LPAREN;
  } else if (c == ')') {
    token.type = TOKEN_RPAREN;
  } else if (c == ' ') {
    token.type = TOKEN_EOF;
  } else {
    token.type = TOKEN_NUMBER;
    token.value = c;
  }
  return token;
}
#if 1
// testing
int main(void) {
  const char *src = "3+4";
  Token token[10];
  for (int i = 0; i < 4; i++) {
    token[i] = next_token(&src);
    printf("%d", token[i].type);
  }
  printf("\n");

  return 0;
}
#endif
