#include "Lexer.h"
#include "String.h"
#include <assert.h>
#include <stdio.h>

int printString(String *s) { return printf("%.*s\n", (int)s->len, s->arr); }

int main(int argc, char *argv[]) {
    assert(argc == 2 && "Usage: main <input>");
    TokensList tokens = {0};
    ErrCode err = scanFile(&tokens, argv[1]);
    for (usize i = 0; i < tokens.len; i++) {
        printToken(tokens.arr[i]);
    }
    return err;
}
