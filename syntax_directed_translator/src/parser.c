#include "parser.h"
#include "lexer.h"
#include <limits.h>
#include <string.h>

// TODO: implement comparison operators:
char *current_result_temp = NULL;
struct Result expr();
void stmt(void); /**/
int tmp_count = 0;

//  handles ()
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
    } else {
      fprintf(stderr, "ERROR: '%s' used before assignement\n", val.var_name);
      exit(1);
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
  while (lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_STAR ||
         lookahead.type == TOKEN_MODULO) {
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
    } else if (token == TOKEN_MODULO) {
      left.value %= right.value;
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
// check for comparison functions return int min if no comparison operator is
// existent
struct Result compar(void) {
  struct Result left = expr();
  if (lookahead.type == TOKEN_EQ || lookahead.type == TOKEN_NE ||
      lookahead.type == TOKEN_LT || lookahead.type == TOKEN_LE ||
      lookahead.type == TOKEN_GT || lookahead.type == TOKEN_GE) {
    struct Result res;
    TokenType token = lookahead.type;
    advance();
    struct Result right = expr();
    // CURRNETly < and > work not the rest?
    if (token == TOKEN_EQ)
      res.value = left.value == right.value;
    else if (token == TOKEN_NE)
      res.value = left.value != right.value;
    else if (token == TOKEN_LT)
      res.value = left.value < right.value;
    else if (token == TOKEN_LE)
      res.value = left.value <= right.value;
    else if (token == TOKEN_GT)
      res.value = left.value > right.value;
    else if (token == TOKEN_GE)
      res.value = left.value >= right.value;
    res.val_is_bool = true;
    return res;
  }
  return left;
}
// check if it is a stmt
void stmt(void) {
  if (lookahead.type == TOKEN_IDENT) {
    char name[64];
    strcpy(name, lookahead.name);
    advance();
    if (lookahead.type == TOKEN_ASSIGN) {
      // printf("%s ", token_to_string(TOKEN_ASSIGN));
      advance();
      struct Result res = compar();
      printf("%s = ", name);
      if (res.is_literal) {
        printf("%d\n", res.value);
      } else {
        printf("%s\n", res.var_name);
      }
      symtable_set(name, res.value);
      printf("RESULT: %d\n", res.value);
    } else if (lookahead.type == TOKEN_EXIT) {
      advance();
    } else {
      puts("error falsche grammatik");
    }
  } else {
    struct Result res = compar();
    printf("RESULT: %d\n", res.value);
  }
  if (lookahead.type == TOKEN_COMMA) {
    advance();
    stmt();
  }
}

#if 0
//DEBUG:
Token lookahead;
const char *cursor;
int main(void) {
  cursor = " x = (4/4)*2; y = x+2";
  advance();
  stmt();
  return 0;
}
#endif
