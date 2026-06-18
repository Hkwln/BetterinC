#include "lexer.h"
#include <stdbool.h>
#include <string.h>

// helper function to check if the char is a digit
bool is_digit(char c) {
  if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' ||
      c == '7' || c == '8' || c == '9' || c == '0')
    return 1;
  else
    return 0;
}
bool is_letter(char c) {
  if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' ||
      c == 'g' || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' ||
      c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' ||
      c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' ||
      c == 'y' || c == 'z' || c == 'A' || c == 'B' || c == 'C' || c == 'D' ||
      c == 'E' || c == 'F' || c == 'G' || c == 'H' || c == 'I' || c == 'J' ||
      c == 'K' || c == 'L' || c == 'M' || c == 'N' || c == 'O' || c == 'P' ||
      c == 'Q' || c == 'R' || c == 'S' || c == 'T' || c == 'U' || c == 'V' ||
      c == 'W' || c == 'X' || c == 'Y' || c == 'Z')
    return 1;
  return false;
}
// checks what type is next token
Token next_token(const char **src) {
  Token token;
  char c = **src;
  if (c == '\0') {
    token.type = TOKEN_EOF;
    return token;
  }
  (*src)++;
  if (c == ' ' || c == '\n') {
    return next_token(src);
  }
  if (c == ';') {
    token.type = TOKEN_COMMA;
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
  } else if (c == '%') {
    token.type = TOKEN_MODULO;
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
  } else if (c == '=') {
    token.type = TOKEN_ASSIGN;
  } else if (is_digit(c)) {
    token.type = TOKEN_NUMBER;
    char c2 = **src;
    int num1 = c - '0';
    while (is_digit(**src)) {
      int num2 = c2 - '0';
      num1 = (num1 * 10) + num2;
      (*src)++;
      c2 = **src;
    }
    token.value = num1;
    return token;
  } else if (c == '=') {
    token.type = TOKEN_ASSIGN;
  } else if (is_letter(c) || c == '_') {
    token.type = TOKEN_IDENT;
    int i = 0;
    token.name[i++] = c;
    while (is_letter(**src) || **src == '_') {
      token.name[i++] = **src;
      (*src)++;
    }
    token.name[i] = '\0';
    if (strcmp(token.name, "exit") == 0) {
      token.type = TOKEN_EXIT;
    }
    return token;
  }
  return token;
}
const char *token_to_string(TokenType type) {
  switch (type) {
  case (TOKEN_PLUS):
    // return "PLUS, ";
    return "+";
  case (TOKEN_MINUS):
    // return "MINUS, ";
    return "-";
  case (TOKEN_STAR):
    // return "STAR, ";
    return "*";
  case (TOKEN_MODULO):
    // return "MODULO, ";
    return "%";
  case (TOKEN_SLASH):
    // return "SLASH, ";
    return "/";
  case (TOKEN_LPAREN):
    return "Lparen, ";
  case (TOKEN_RPAREN):
    return "Rparen, ";
  case (TOKEN_NUMBER):
    return "Number";
  case (TOKEN_EOF):
    return "EOF";
  case (TOKEN_ASSIGN):
    // return "ASSIGN, ";
    return "=";
  case (TOKEN_IDENT):
    return "IDENT";
  default:
    return "UNKNOWN";
  }
}
#if 0
#include <stdio.h>
#include <string.h>
// testing
int main(void) {
  const char *src = "x = y * 4";
  Token token[10];
  int i = 0;
  do {
    token[i] = next_token(&src);
    const char *str = token_to_string(token[i].type);
    if (strcmp(str, "Number") == 0) {
      printf("%s(%d), ", str, token[i].value);
    } else if (strcmp(str, "IDENT") == 0) {
      printf("%s(%s), ", str, token[i].name);
    } else {
      printf("%s", str);
    }
    i++;
  } while (token[i - 1].type != TOKEN_EOF);
  printf("\n");

  return 0;
}
#endif
