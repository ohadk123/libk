#include "Token.h"
#include <stdio.h>

#define X(type) [type] = #type,
cstr tokenTypesStrings[] = { TOKEN_LIST };
#undef X

Token makeSimple(TokenType type, usize line, usize col) {
    return (Token) {
        .type = type,
        .as.unknown = 0,
        .line = line,
        .col = col,
    };
}

Token makeUnknown(u8 c, usize line, usize col) {
    return (Token) {
        .type = TOK_UNKNOWN,
        .as.unknown = c,
        .line = line,
        .col = col,
    };
}

Token makeIdentifierToken(String ident, usize line, usize col) {
    return (Token) {
        .type = TOK_IDENTIFIER,
        .as.identifier = ident,
        .line = line,
        .col = col,
    };
}

Token makeStringLiteralToken(String strLit, usize line, usize col) {
    return (Token) {
        .type = TOK_STRING_LITERAL,
        .as.stringLiteral = strLit,
        .line = line,
        .col = col,
    };
}

Token makeIntegerLiteralToken(u64 value, usize line, usize col) {
    return (Token) {
        .type = TOK_INTEGER_LITERAL,
        .as.integerLiteral = value,
        .line = line,
        .col = col,
    };
}

Token makeFloatLiteralToken(f64 value, usize line, usize col) {
    return (Token) {
        .type = TOK_FLOAT_LITERAL,
        .as.floatLiteral = value,
        .line = line,
        .col = col,
    };
}

Token makeCharLiteralToken(u8 value, usize line, usize col) {
    return (Token) {
        .type = TOK_CHAR_LITERAL,
        .as.charLiteral = value,
        .line = line,
        .col = col,
    };
}

Token makeErrorToken(String errorMsg, usize line, usize col) {
    return (Token) {
        .type = TOK_ERROR,
        .as.error = errorMsg,
        .line = line,
        .col = col,
    };
}

static Bool isPrintableChar(u8 c) {
    return ' ' <= c && c <= '~';
}

void printToken(Token token) {
    printf("[%lu:%lu = %s]", token.line, token.col, tokenTypesStrings[token.type]);
    switch (token.type) {
    case TOK_IDENTIFIER:
        printf(": (%.*s)\n", (int)token.as.identifier.len, token.as.identifier.arr);
        return;
    case TOK_STRING_LITERAL:
        printf(": (%.*s)\n", (int)token.as.stringLiteral.len, token.as.stringLiteral.arr);
        return;
    case TOK_CHAR_LITERAL:
        printf(": (0x%02x)", token.as.charLiteral);
        if (isPrintableChar(token.as.charLiteral)) printf(" : \'%c\'\n", token.as.charLiteral);
        else printf("\n");
        break;
    case TOK_INTEGER_LITERAL:
        printf(": (%lu)\n", token.as.integerLiteral);
        return;
    case TOK_FLOAT_LITERAL:
        printf(": (%f)\n", token.as.floatLiteral);
        return;
    case TOK_UNKNOWN:
        printf(": (0x%02x)", token.as.unknown);
        if (isPrintableChar(token.as.unknown)) printf(" : \'%c\'\n", token.as.unknown);
        else printf("\n");
        break;
    case TOK_ERROR:
        printf(": (%.*s)\n", (int) token.as.error.len, token.as.error.arr);
        break;
    default:
        printf("\n");
        return;
    }
}
