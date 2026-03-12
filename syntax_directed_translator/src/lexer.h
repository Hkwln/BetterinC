#pragma once
typedef enum {
  TOKEN_NUMBER,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  int value; /* nur relevant wenn type == TOKEN_NUMBER */
} Token;
Token next_token(const char **src);
const char *token_to_string(TokenType type);
