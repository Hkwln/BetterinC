#include "lexer.h"
#include <stdio.h>
#include <string.h>
/*
Implementiere `Token next_token(const char **src)` — sie liest das nächste Token
aus `*src` und rückt den Zeiger vor. Leerzeichen überspringen.
 * */
// TODO: what if the number is 12? both 1 and 2 are read seperated;
Token next_token(const char **src) {
  Token token;
  char c = **src;
  if (c == '\0') {
    token.type = TOKEN_EOF;
    return token;
  }
  (*src)++;
  if (c == ' ') {
    next_token(src);
    return token;
  }
  if (c == '+') {
    token.type = TOKEN_PLUS;
    return token;
  } else if (c == '-') {
    token.type = TOKEN_MINUS;
    return token;
  } else if (c == '*') {
    token.type = TOKEN_STAR;
    return token;
  } else if (c == '/') {
    token.type = TOKEN_SLASH;
    return token;
  } else if (c == '(') {
    token.type = TOKEN_LPAREN;

    return token;
  } else if (c == ')') {
    token.type = TOKEN_RPAREN;
    return token;
  } else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||
             c == '6' || c == '7' || c == '8' || c == '9' || c == '0') {
    token.type = TOKEN_NUMBER;
    char c2 = **src;
    int num1 = c - '0';
    while (**src == '1' || **src == '2' || **src == '3' || **src == '4' ||
           **src == '5' || c2 == '6' || c2 == '7' || **src == '8' ||
           **src == '9' || **src == '0') {
      int num2 = c2 - '0';
      num1 = (num1 * 10) + num2;
      (*src)++;
      c2 = **src;
    }
    token.value = num1;
    return token;
  }
  return token;
}
const char *token_to_string(TokenType type) {
  switch (type) {
  case (TOKEN_PLUS):
    return "PLUS, ";
  case (TOKEN_MINUS):
    return "MINUS, ";
  case (TOKEN_STAR):
    return "STAR, ";
  case (TOKEN_SLASH):
    return "SLASH, ";
  case (TOKEN_LPAREN):
    return "Lparen, ";
  case (TOKEN_RPAREN):
    return "Rparen, ";
  case (TOKEN_NUMBER):
    return "Number";
  case (TOKEN_EOF):
    return "EOF";
  default:
    return "UNKNOWN";
  }
}
#if 0
// testing
int main(void) {
  const char *src = "(10/4)+5";
  Token token[10];
  int i = 0;
  do {
    token[i] = next_token(&src);
    const char *str = token_to_string(token[i].type);
    if (strcmp(str, "Number") == 0) {
      printf("%s(%d),  ", str, token[i].value);
    } else {
      printf("%s", str);
    }
    i++;
  } while (token[i - 1].type != TOKEN_EOF);
  printf("\n");

  return 0;
}
#endif
