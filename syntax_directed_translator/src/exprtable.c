#include "exprtable.h"

#include "lexer.h"
#include "parser.h"

int label_count = 0;

struct Result parse_expr()
{
    struct Result res;
    if (lookahead.type == TOKEN_IF || lookahead.type == TOKEN_WHILE) {
        TokenType token = lookahead.type;
        printf("%s", token_to_string(TOKEN_IF));
        advance();
        if (lookahead.type == TOKEN_LPAREN) {
            struct Result B;
            struct Result S1;
            if (token == TOKEN_IF) {
                if ((B = compar(token)).value == 1) {
                    match(TOKEN_RPAREN);
                    S1 = expr();
                } else if (token == TOKEN_WHILE)
                    while ((B = compar(token)).value == 1) {
                        match(TOKEN_RPAREN);
                        S1 = expr();
                    }
            }
            printf("%s: ", B.f_label);
            res.value = S1.value;
        } else {
            fprintf(stderr, "error false if grammar\n ");
            exit(1);
        }
    }

    return res;
}
