#pragma once
#include "lexer.h"
extern Token lookahead;
extern const char *cursor;
static inline void advance() { lookahead = next_token(&cursor); }
int expr(void);   /* verarbeitet + und - */
int term(void);   /* verarbeitet * und / */
int factor(void); /* verarbeitet Zahlen und geklammerte Ausdrücke */
void stmt(void);  /* verarbeitet: variable = zuweisung*/
