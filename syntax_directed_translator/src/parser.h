#pragma once
#include "lexer.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

extern Token lookahead;
extern const char *cursor;
extern int tmp_count;
static inline void advance() { lookahead = next_token(&cursor); }

struct Result {
  char var_name[64];
  int value;
  bool is_literal;
  bool val_is_bool;
};

/* INFO:helper functions:*/
static inline char *new_temp(void) {
  static char buf[128];
  static int offset = 0;
  char *result = buf + offset;
  offset += sprintf(result, "t%d", ++tmp_count) + 1;
  if (offset >= 120)
    offset = 0;
  return result;
}
static inline void match(TokenType expected) {
  if (lookahead.type != expected) {
    fprintf(stderr, "error this does not match \n");
    exit(1);
  } else {
    advance();
  }
}
struct Result expr(void);   /* verarbeitet + und - */
struct Result term(void);   /* verarbeitet * und /  und %*/
struct Result factor(void); /* verarbeitet Zahlen und geklammerte Ausdrücke */
void stmt(void);            /* verarbeitet: variable = zuweisung*/
