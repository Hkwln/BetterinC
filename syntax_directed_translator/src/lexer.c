#include "lexer.h"

#include <string.h>

// checks what type is next token
Token next_token(const char** src)
{
    Token token;
    char c = **src;
    if (c == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    (*src)++;
    if (c == ' ' || c == '\n') {
        return next_token(src);
    }
    if (c == ';') {
        token.type = TOKEN_COMMA;
        return token;
    }
    // operations:
    if (c == '+') {
        token.type = TOKEN_PLUS;
        return token;
    } else if (c == '-') {
        token.type = TOKEN_MINUS;
        return token;
    } else if (c == '*') {
        token.type = TOKEN_STAR;
        return token;
    } else if (c == '%') {
        token.type = TOKEN_MODULO;
        return token;
    }
    //
    else if (c == '/') {
        token.type = TOKEN_SLASH;
        return token;
    } else if (c == '(') {
        token.type = TOKEN_LPAREN;
        return token;
    } else if (c == ')') {
        token.type = TOKEN_RPAREN;
        return token;
    } else if (is_digit(c)) {
        token.type = TOKEN_NUMBER;
        char c2 = **src;
        int num1 = c - '0';
        while (is_digit(**src)) {
            int num2 = c2 - '0';
            num1 = (num1 * 10) + num2;
            (*src)++;
            c2 = **src;
        }
        token.value = num1;
        return token;
    } else if (is_letter(c) || c == '_') {
        token.type = TOKEN_IDENT;
        int i = 0;
        token.name[i++] = c;
        while (is_letter(**src) || **src == '_') {
            token.name[i++] = **src;
            (*src)++;
        }
        token.name[i] = '\0';
        if (strcmp(token.name, "exit") == 0) {
            token.type = TOKEN_EXIT;
        } else if (strcmp(token.name, "while") == 0) {
            token.type = TOKEN_WHILE;
        } else if (strcmp(token.name, "if") == 0) {
            token.type = TOKEN_IF;
        }
        return token;
    }
    //  comparison operators:
    else if (c == '=') {
        token.type = TOKEN_ASSIGN;
        if (**src == '=') {
            token.type = TOKEN_EQ;
            (*src)++;
        }
        return token;
    } else if (c == '<') {
        token.type = TOKEN_LT;
        if (**src == '=') {
            token.type = TOKEN_LE;
            (*src)++;
        }
        return token;
    } else if (c == '>') {
        token.type = TOKEN_GT;
        if (**src == '=') {
            token.type = TOKEN_GE;
            (*src)++;
        }
        return token;
    } else if (c == '!') {  // needs a fix 4!= 3 RESULT: 4
        token.type = TOKEN_NOT;
        if (**src == '=') {
            token.type = TOKEN_NE;
            (*src)++;
        }
        return token;
    }
    return token;
}

#if 1
// testing
#include <stdio.h>
int main(void)
{
    const char* src = " if() while ";
    Token token[10];
    int i = 0;
    do {
        token[i] = next_token(&src);
        const char* str = token_to_string(token[i].type);
        if (strcmp(str, "Number") == 0) {
            printf("%s(%d), ", str, token[i].value);
        } else if (strcmp(str, "IDENT") == 0) {
            printf("%s(%s), ", str, token[i].name);
        } else {
            printf("%s", str);
        }
        i++;
    } while (token[i - 1].type != TOKEN_EOF);
    printf("\n");

    return 0;
}
#endif
