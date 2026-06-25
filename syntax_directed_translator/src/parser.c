#include "parser.h"

#include <limits.h>
#include <string.h>

#include "lexer.h"

// TODO: implement if/while statement
char* current_result_temp = NULL;
struct Result expr();
void stmt(void); /**/
int tmp_count = 0;
int label_count = 0;

//  handles () numbers and identifiers
struct Result factor(void)
{
    struct Result val;
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
            fprintf(stderr, "ERROR: '%s' used before assignement\n",
                    val.var_name);
            exit(1);
        }
    } else if (lookahead.type == TOKEN_LPAREN) {
        // maybe check if this expression was there before
        advance();
        val = compar(0);
        match(TOKEN_RPAREN);
        return val;
    } else {
        exit(1);
    }
    return val;
}
// handles */
struct Result term(void)
{
    struct Result left = factor();
    while (lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_STAR ||
           lookahead.type == TOKEN_MODULO) {
        TokenType token = lookahead.type;
        advance();
        struct Result right = factor();
        char* temp = new_temp();
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
struct Result expr(void)
{
    struct Result left = term();
    while (lookahead.type == TOKEN_MINUS || lookahead.type == TOKEN_PLUS) {
        TokenType token = lookahead.type;
        advance();
        struct Result right = term();
        char* temp = new_temp();
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
struct Result compar(TokenType statement)
{
    struct Result left = expr();
    if (lookahead.type == TOKEN_EQ || lookahead.type == TOKEN_NE ||
        lookahead.type == TOKEN_LT || lookahead.type == TOKEN_LE ||
        lookahead.type == TOKEN_GT || lookahead.type == TOKEN_GE) {
        struct Result res;
        TokenType token = lookahead.type;
        advance();
        struct Result right = expr();
        if (statement == 0) {
            char* temp = new_temp();
            strcpy(res.var_name, temp);
            printf("%s = ", temp);
            if (left.is_literal) {
                printf("%d", left.value);
            } else {
                printf("%s", left.var_name);
            }
            printf(" %s ", token_to_string(token));
            if (right.is_literal) {
                printf("%d", right.value);
            } else {
                printf("%s", right.var_name);
            }
            printf("\n");
        } else if (statement == TOKEN_IF || statement == TOKEN_WHILE) {
            if (statement == TOKEN_WHILE) {
                printf("%s: begin", new_label());
            }
            // this is a if or an else statement
            res.t_label = new_label();
            res.f_label = new_label();
            printf("%s %s %s\n", left.var_name, token_to_string(token),
                   right.var_name);
            printf("goto %s\ngoto %s", res.t_label, res.f_label);
            printf("%s: ", res.t_label);
        }
        if (token == TOKEN_EQ)
            res.value = left.value == right.value;
        else if (token == TOKEN_NE) {
            res.value = left.value != right.value;
        } else if (token == TOKEN_LT)
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
    /* TODO:
    else if (lookahead.type == TOKEN_ASSIGN) {
      struct Result res = expr();
    }
      */
    return left;
}

// check if it is a stmt
void stmt(void)
{
    if (lookahead.type == TOKEN_IDENT) {
        char name[64];
        strcpy(name, lookahead.name);
        advance();
        if (lookahead.type == TOKEN_ASSIGN) {
            // printf("%s ", token_to_string(TOKEN_ASSIGN));
            advance();
            struct Result res = compar(0);
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
    } else if (lookahead.type == TOKEN_COMMA) {
        advance();
        stmt();
    } else if (lookahead.type == TOKEN_IF || lookahead.type == TOKEN_WHILE) {
        struct Result res;
        TokenType token = lookahead.type;
        printf("%s", token_to_string(TOKEN_IF));
        advance();
        if (lookahead.type == TOKEN_LPAREN) {
            struct Result B;
            struct Result S1;
            if (token == TOKEN_IF) {
                // FIX: don't evaluate at runtime
                B = compar(token);
                match(TOKEN_RPAREN);
                if (B.value == 1) {
                    S1 = expr();
                }
            } else if (token == TOKEN_WHILE) {
                // FIX: don't evaluate at runtime
                B = compar(token);
                match(TOKEN_RPAREN);
                while (B.value == 1) {
                    S1 = expr();
                }
            }
            printf("%s: ", B.f_label);
            res.value = S1.value;
        } else {
            fprintf(stderr, "error false if grammar\n ");
            exit(1);
        }
        printf("RESULT: %d\n", res.value);
    } else {
        struct Result res = compar(0);
        printf("RESULT: %d\n", res.value);
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
