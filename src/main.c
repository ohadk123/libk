#include "Lexer.h"
#include "Parser.h"
#include "String.h"
#include <assert.h>
#include <stdio.h>

int printString(String *s) { return printf("%.*s\n", (int)s->len, s->arr); }

int eval(Expr *root) {
    int lhs, rhs;

    switch (root->type) {
        case EXPR_NUM:
            return root->as.num.as.integerLiteral;
        case EXPR_BINARY:
            switch (root->as.binary.op) {
                case TOK_PLUS:
                    lhs = eval(root->as.binary.lhs);
                    rhs = eval(root->as.binary.rhs);
                    return lhs + rhs;
                case TOK_STAR:
                    lhs = eval(root->as.binary.lhs);
                    rhs = eval(root->as.binary.rhs);
                    return lhs * rhs;
                default:
                    TODO("Binary Operators");
            }
        default:
            TODO("Expressions");
            break;
    }
}

int main(int argc, char *argv[]) {
    assert(argc == 2 && "Usage: main <input>");
    TokensList tokens = {0};
    String fileName = {0};
    joinCString(&fileName, argv[1]);
    joinByte(&fileName, '\0');

    ErrCode err = scanFile(&tokens, fileName);
    for (usize i = 0; i < tokens.len; i++) {
        printToken(tokens.arr[i]);
    }

    Expr *root = parseTokens(tokens);
    int result = eval(root);
    printf("result = %d\n", result);
    freeExpr(root);

    return err;
}
