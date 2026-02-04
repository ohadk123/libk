#include "Lexer.h"
#include "String.h"
#include <assert.h>
#include <stdio.h>

int printString(String *s) { return printf("%.*s\n", (int)s->len, s->arr); }

int main(int argc, char *argv[]) {
    assert(argc == 2 && "Usage: lexer <source-file>");
    TokensList tokens = {0};
    ErrCode err = scanFile(&tokens, argv[1]);
    printf("err = %d\n", err);
    for (usize i = 0; i < tokens.len; i++) {
        Token *token = &tokens.arr[i];
        printToken(*token);
    }

    freeTokensList(&tokens);
    return err;
}
