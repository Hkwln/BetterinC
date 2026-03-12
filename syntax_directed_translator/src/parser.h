#pragma once
#include "lexer.h"
extern Token lookahead;
extern const char *cursor;

static inline void advance() { lookahead = next_token(&cursor); }
void expr(void);   /* verarbeitet + und - */
void term(void);   /* verarbeitet * und / */
void factor(void); /* verarbeitet Zahlen und geklammerte Ausdrücke */
