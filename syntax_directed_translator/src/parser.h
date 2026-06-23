#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "symtable.h"

extern Token lookahead;
extern const char* cursor;
extern int tmp_count;
extern int label_count;
static inline void advance()
{
    lookahead = next_token(&cursor);
}

struct Result {
    char var_name[64];
    int value;
    bool is_literal;
    bool val_is_bool;
    char* t_label;
    char* f_label;
};

/* INFO:helper functions:*/
static inline char* new_temp(void)
{
    static char buf[128];
    static int offset = 0;
    char* result = buf + offset;
    offset += sprintf(result, "t%d", ++tmp_count) + 1;
    if (offset >= 120) offset = 0;
    return result;
}
// the same as above but with a different counter and L instead of t
static inline char* new_label(void)
{
    static char buf[128];
    static int offset = 0;
    char* result = buf + offset;
    offset += sprintf(result, "L%d", ++label_count) + 1;
    if (offset >= 120) offset = 0;
    return result;
}

static inline void match(TokenType expected)
{
    if (lookahead.type != expected) {
        fprintf(stderr, "ERROR: don't got the expected expected %s \n",
                token_to_string(expected));
        exit(1);
    } else {
        advance();
    }
}
struct Result expr(void);   /* verarbeitet + und - */
struct Result term(void);   /* verarbeitet * und /  und %*/
struct Result factor(void); /* verarbeitet Zahlen und geklammerte Ausdrücke */
struct Result compar(TokenType statement); /* verarbeitet == != < <= > >= */
void stmt(void); /* verarbeitet: variable = zuweisung*/
