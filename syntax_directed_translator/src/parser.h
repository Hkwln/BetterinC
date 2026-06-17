#pragma once
#include "lexer.h"
#include "symtable.h"
#include <stdbool.h>

extern Token lookahead;
extern const char *cursor;
static inline void advance() { lookahead = next_token(&cursor); }

struct Result {
  char var_name[64];
  int value;
  bool is_literal;
};
struct Result expr(void);   /* verarbeitet + und - */
struct Result term(void);   /* verarbeitet * und /  und %*/
struct Result factor(void); /* verarbeitet Zahlen und geklammerte Ausdrücke */
void stmt(void);            /* verarbeitet: variable = zuweisung*/
