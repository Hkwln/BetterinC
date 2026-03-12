#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

// global variables:
Token lookahead;
const char *cursor;
void expr();
void advance() { lookahead = next_token(&cursor); }
void match(TokenType expected) {
  if (lookahead.type != expected) {
    fprintf(stderr, "error this does not match \n");
    exit(1);
  } else {
    advance();
  }
}
// handles ()
void factor(void) {
  if (lookahead.type == TOKEN_NUMBER) {
    printf("%d, ", lookahead.value);
    advance();
  } else if (lookahead.type == TOKEN_LPAREN) {
    advance();
    expr();
    match(TOKEN_RPAREN);
  } else {
    exit(1);
  }
}
// handles */
void term(void) {
  factor();
  while (lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_STAR) {
    TokenType token = lookahead.type;
    advance();
    factor();
    // do something
    printf("%s", token_to_string(token));
  }
}
void expr(void) {
  term();
  while (lookahead.type == TOKEN_MINUS || lookahead.type == TOKEN_PLUS) {
    // do something
    TokenType token = lookahead.type;
    advance();
    term();
    printf("%s", token_to_string(token));
  }
  if (lookahead.type == TOKEN_EOF) {
    printf("\n");
    return;
  }
}
#if 1
int main(void) {
  cursor = "(1/4)*2";
  advance();
  expr();
  return 0;
}
#endif
