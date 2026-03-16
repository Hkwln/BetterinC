#include "parser.h"
#include "lexer.h"
#include "symtable.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int varval = 0;
int expr();
void stmt(void); /**/
/*INFO: erste zwei funktionen sind hilffunktionen:*/
int tmp_count = 0;
char *new_temp(void) {
  // number in char:
  return strcat("t", (char *)(tmp_count++ + '0'));
}

void match(TokenType expected) {
  if (lookahead.type != expected) {
    fprintf(stderr, "error this does not match \n");
    exit(1);
  } else {
    advance();
  }
}
// handles ()
int factor(void) {
  if (lookahead.type == TOKEN_NUMBER) {
    int val = lookahead.value;
    printf("%d ", val);
    advance();
    return val;
  } else if (lookahead.type == TOKEN_IDENT) {
    int value = symtable_get(lookahead.name);
    advance();
    if (value != INT_MIN) {
      printf("%d ", value);
      return value;
    } else {
      printf("%s ", lookahead.name);
    }
  } else if (lookahead.type == TOKEN_LPAREN) {
    advance();
    int val = expr();
    match(TOKEN_RPAREN);
    return val;
  } else {
    exit(1);
  }
  return 0;
}
// handles */
int term(void) {
  int result = factor();
  while (lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_STAR) {
    TokenType token = lookahead.type;
    advance();
    int right = factor();
    // do something
    printf("%s ", token_to_string(token));
    if (token == TOKEN_SLASH) {
      result /= right;
    } else if (token == TOKEN_STAR) {
      result *= right;
    }
  }
  return result;
}
int expr(void) {
  int result = term();
  while (lookahead.type == TOKEN_MINUS || lookahead.type == TOKEN_PLUS) {
    // do something
    TokenType token = lookahead.type;
    advance();
    int right = term();
    printf("%s ", token_to_string(token));
    if (token == TOKEN_MINUS) {
      result -= right;
    } else if (token == TOKEN_PLUS) {
      result += right;
    }
  }
  if (lookahead.type == TOKEN_EOF) {
    printf("\n");
  }
  return result;
}
// check if it is a stmt
void stmt(void) {
  if (lookahead.type == TOKEN_IDENT) {
    printf("%s ", lookahead.name);
    // now we expect a =
    char name[64];
    strcpy(name, lookahead.name);
    advance();
    if (lookahead.type == TOKEN_ASSIGN) {
      printf("%s ", token_to_string(TOKEN_ASSIGN));
      advance();
      varval = expr();
      symtable_set(name, varval);
    } else {
      printf("error falsche grammatik");
    }
  } else {
    expr();
  }
  if (lookahead.type == TOKEN_COMMA) {
    printf("\n");
    advance();
    stmt();
  }
}
#if 1
Token lookahead;
const char *cursor;
int main(void) {
  cursor = " x = (4/4)*2; y = x+2";
  advance();
  stmt();
  return 0;
}
#endif
