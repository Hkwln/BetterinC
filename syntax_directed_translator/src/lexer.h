#pragma once
#include <stdbool.h>

typedef enum {
  // operations:
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_MODULO,
  //
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EOF,
  TOKEN_IDENT,
  TOKEN_NUMBER,
  TOKEN_COMMA,
  TOKEN_EXIT,
  // comparison operators:
  TOKEN_ASSIGN, // =
  TOKEN_EQ,     // ==
  TOKEN_NOT,    // !
  TOKEN_NE,     //!=
  TOKEN_LT,     // <
  TOKEN_LE,     // <=
  TOKEN_GT,     // >
  TOKEN_GE,     // >=
  // not yet active
  TOKEN_AND, // &&
  TOKEN_OR,  // ||
  // statements:
  TOKEN_WHILE,
  TOKEN_IF
} TokenType;

typedef struct {
  TokenType type;
  int value;     /* nur relevant wenn type == TOKEN_NUMBER */
  char name[64]; // nur releveant wenn es eine type == TOKEN_IDENT
} Token;

Token next_token(const char **src);

//  INFO: helper function to check if the char is a digit
static inline bool is_digit(char c) {
  if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' ||
      c == '7' || c == '8' || c == '9' || c == '0')
    return 1;
  else
    return 0;
}
static inline bool is_letter(char c) {
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

static inline const char *token_to_string(TokenType type) {
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
    // return "Lparen";
    return "(";
  case (TOKEN_RPAREN):
    // return "Rparen";
    return ")";
  case (TOKEN_NUMBER):
    return "Number";
  case (TOKEN_EOF):
    return "EOF";
  case (TOKEN_IDENT):
    return "IDENT";
  // comparison operators:
  case (TOKEN_ASSIGN):
    // return "ASSIGN, ";
    return "=";
  case (TOKEN_EQ):
    return "==";
  case (TOKEN_NOT):
    return "!";
  case (TOKEN_NE):
    return "!=";
  case (TOKEN_LT):
    return "<";
  case (TOKEN_LE):
    return "<=";
  case (TOKEN_GT):
    return ">";
  case (TOKEN_GE):
    return ">=";
  case (TOKEN_AND):
    return "&&";
  case (TOKEN_OR):
    return "||";
  case (TOKEN_WHILE):
    return "while";
  case (TOKEN_IF):
    return "if";
  default:
    return "UNKNOWN";
  }
}
