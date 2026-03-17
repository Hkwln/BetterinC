#include "parser.h"
#include "lexer.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *current_result_temp = NULL;
struct Result expr();
void stmt(void); /**/
int tmp_count = 0;
/* INFO: erste zwei funktionen sind hilffunktionen:*/
char *new_temp(void) {
  static char buf[128];
  static int offset = 0;
  char *result = buf + offset;
  offset += sprintf(result, "t%d", ++tmp_count) + 1;
  if (offset >= 120) offset = 0;
  return result;
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
struct Result factor(void) {
  struct Result val = {0};
  if (lookahead.type == TOKEN_NUMBER) {
    val.value = lookahead.value;
    val.is_literal = true;
    advance();
    return val;
  } else if (lookahead.type == TOKEN_IDENT) {
    val.value = symtable_get(lookahead.name);
    strcpy(val.var_name, lookahead.name);
    val.is_literal = false;
    advance();
    if (val.value != INT_MIN) {
      return val;
    }
  } else if (lookahead.type == TOKEN_LPAREN) {
    advance();
    val = expr();
    match(TOKEN_RPAREN);
    return val;
  } else {
    exit(1);
  }
  return val;
}
// handles */
struct Result term(void) {
  struct Result left = factor();
  while (lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_STAR) {
    TokenType token = lookahead.type;
    advance();
    struct Result right = factor();
    char *temp = new_temp();
    printf("%s = ", temp);
    if (left.is_literal) {
      printf("%d", left.value);
    } else {
      printf("%s", left.var_name);
    }
    printf(" %s ", token_to_string(token));
    if (right.is_literal) {
      printf("%d\n", right.value);
    } else {
      printf("%s\n", right.var_name);
    }
    strcpy(left.var_name, temp);
    left.is_literal = false;
    if (token == TOKEN_SLASH) {
      left.value /= right.value;
      symtable_set(left.var_name, left.value);
    } else if (token == TOKEN_STAR) {
      left.value *= right.value;
      symtable_set(left.var_name, left.value);
    }
  }
  return left;
}
struct Result expr(void) {
  struct Result left = term();
  while (lookahead.type == TOKEN_MINUS || lookahead.type == TOKEN_PLUS) {
    TokenType token = lookahead.type;
    advance();
    struct Result right = term();
    char *temp = new_temp();
    printf("%s = ", temp);
    if (left.is_literal) {
      printf("%d", left.value);
    } else {
      printf("%s", left.var_name);
    }
    printf(" %s ", token_to_string(token));
    if (right.is_literal) {
      printf("%d\n", right.value);
    } else {
      printf("%s\n", right.var_name);
    }
    strcpy(left.var_name, temp);
    left.is_literal = false;
    if (token == TOKEN_MINUS) {
      left.value -= right.value;
      symtable_set(left.var_name, left.value);
    } else if (token == TOKEN_PLUS) {
      left.value += right.value;
      symtable_set(left.var_name, left.value);
    }
    current_result_temp = temp;
  }
  return left;
}
// check if it is a stmt
void stmt(void) {
  if (lookahead.type == TOKEN_IDENT) {
    // printf("%s ", lookahead.name);
    //  now we expect a =
    char name[64];
    strcpy(name, lookahead.name);
    advance();
    if (lookahead.type == TOKEN_ASSIGN) {
      // printf("%s ", token_to_string(TOKEN_ASSIGN));
      advance();
      struct Result res = expr();
      printf("%s = ", name);
      if (res.is_literal) {
        printf("%d\n", res.value);
      } else {
        printf("%s\n", res.var_name);
      }
      symtable_set(name, res.value);
    } else {
      printf("error falsche grammatik");
    }
  } else {
    expr();
  }
  if (lookahead.type == TOKEN_COMMA) {
    // printf("\n");
    advance();
    stmt();
  }
}
#if 0
Token lookahead;
const char *cursor;
int main(void) {
  cursor = " x = (4/4)*2; y = x+2";
  advance();
  stmt();
  return 0;
}
#endif
