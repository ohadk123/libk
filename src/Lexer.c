#include "Lexer.h"
#include "String.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**********************************************************************************************************************
 * Not lexer specific helper functions
 *********************************************************************************************************************/

Bool isIdentChar(u8 c) {
    return (('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || (c == '_'));
}

Bool isDigit(u8 c) {
    return '0' <= c && c <= '9';
}

u8 convertEscapeChar(String *source, usize *place) {
    if (place == NULL || *place >= source->len-1 || source->arr[*place] != '\\') return -1;

    (*place)++;
    switch (*place) {
        case 'n':  return '\n';
        case 't':  return '\t';
        case 'v':  return '\v';
        case 'b':  return '\b';
        case 'r':  return '\r';
        case 'f':  return '\f';
        case 'a':  return '\a';
        case '\\': return '\\';
        case '?':  return '\?';
        case '\'': return '\'';
        case '\"': return '\"';
        case 'x':
            assert(0 && "Hex escape characters not implemented yet");
        case '0':
            assert(0 && "Octal escape characters not implemented yet");
    }
    return -1;
}

/**********************************************************************************************************************
 * Token functions
 *********************************************************************************************************************/

#define X(type) [type] = #type,
cstr tokenTypesStrings[] = { TOKEN_LIST };
#undef X

Token makeUnknown(u8 c, usize line, usize col) {
    Token token = {0};
    token.type = TOK_UNKNOWN;
    token.as.unknown = c;
    token.line = line;
    token.col = col;
    return token;
}

Token makeToken(TokenType type, usize line, usize col) {
    Token token = {0};
    token.type = type;
    token.line = line;
    token.col = col;
    return token;
}

Token makeIdentifierToken(String ident, usize line, usize col) {
    Token token = {0};
    token.type = TOK_IDENTIFIER;
    token.as.identifier = ident;
    token.line = line;
    token.col = col;
    return token;
}

Token makeStringLiteralToken(String strLit, usize line, usize col) {
    Token token = {0};
    token.type = TOK_STRING_LITERAL;
    token.as.stringLiteral = strLit;
    token.line = line;
    token.col = col;
    return token;
}

Token makeIntegerLiteralToken(u64 value, usize line, usize col) {
    Token token = {0};
    token.type = TOK_INTEGER_LITERAL;
    token.as.integerLiteral = value;
    token.line = line;
    token.col = col;
    return token;
}

Token makeFloatLiteralToken(f64 value, usize line, usize col) {
    Token token = {0};
    token.type = TOK_FLOAT_LITERAL;
    token.as.floatLiteral = value;
    token.line = line;
    token.col = col;
    return token;
}

Token makeCharLiteralToken(u8 value, usize line, usize col) {
    Token token = {0};
    token.type = TOK_CHAR_LITERAL;
    token.as.charLiteral = value;
    token.line = line;
    token.col = col;
    return token;
}

Token makeErrorToken(String errorMsg, usize line, usize col) {
    Token token = {0};
    token.type = TOK_ERROR;
    token.as.error = errorMsg;
    token.line = line;
    token.col = col;
    return token;
}

/**********************************************************************************************************************
 * Lexer functions
 *********************************************************************************************************************/

typedef struct {
    String input;
    usize line;
    usize col;
    usize index;
    TokensList *tokens;
} Lexer;

static void addSimpleToken(Lexer *l, TokenType type) {
    Token token = makeToken(type, l->line, l->col);
    LIST_appendSingle(l->tokens, token);
}

// Check if we've reached the end of the input
static Bool isAtEnd(Lexer *l) {
    return l->index == l->input.len;
}

// Advance the lexer and return the current character
static u8 advance(Lexer *l) {
    if (isAtEnd(l)) return 0;
    l->col++;
    return l->input.arr[l->index++];
}

// Check if the next character matches the expected character and advance if it does
static Bool match(Lexer *l, u8 next) {
    if (isAtEnd(l)) return FALSE;
    if (l->input.arr[l->index] != next) return FALSE;

    l->index++;
    return TRUE;
}

// Peek at the current character without advancing
static u8 peek(Lexer *l) {
    if(isAtEnd(l)) return 0;
    return l->input.arr[l->index];
}

// Peek ahead n characters without advancing
static u8 peekAhead(Lexer *l, usize n) {
    if (l->index + n >= l->input.len) return 0;
    return l->input.arr[l->index + n];
}

/**********************************************************************************************************************
 * Public Lexer API
 *********************************************************************************************************************/

ErrCode scanFile(TokensList *dest, cstr path) {
    if (dest == NULL) return NULLPTR_ERR;
    Lexer lexer = {0};
    lexer.tokens = dest;
    lexer.line = 1;

    ErrCode err = joinEntireFile(&lexer.input, path);
    if (err != NO_ERR) return err;

    while (!isAtEnd(&lexer)) {
        u8 c = advance(&lexer);

        switch (c) {
        case '\n':
            lexer.line++;
            lexer.col = 0;
        case ' ':
        case '\r':
        case '\t':
            break;
        // TOK_EQUALS,
        // TOK_EQUAL_EQUALS,
        case '=':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_EQUAL_EQUALS);
            else addSimpleToken(&lexer, TOK_EQUALS);
            break;
        // TOK_PLUS,
        // TOK_PLUS_PLUS,
        // TOK_PLUS_EQUALS,
        case '+':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_PLUS_EQUALS);
            else if (match(&lexer, '+')) addSimpleToken(&lexer, TOK_PLUS_PLUS);
            else addSimpleToken(&lexer, TOK_PLUS);
            break;
        // TOK_MINUS,
        // TOK_MINUS_MINUS,
        // TOK_MINUS_EQUALS,
        // TOK_MINUS_GREATER,
        case '-':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_MINUS_EQUALS);
            else if (match(&lexer, '-')) addSimpleToken(&lexer, TOK_MINUS_MINUS);
            else if (match(&lexer, '>')) addSimpleToken(&lexer, TOK_MINUS_GREATER);
            else addSimpleToken(&lexer, TOK_MINUS);
            break;
        // TOK_STAR,
        // TOK_STAR_EQUALS,
        case '*':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_STAR_EQUALS);
            else addSimpleToken(&lexer, TOK_STAR);
            break;
        // TOK_SLASH,
        // TOK_SLASH_EQUALS,
        case '/':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_SLASH_EQUALS);
            else addSimpleToken(&lexer, TOK_SLASH);
            break;
        // TOK_PERCENT,
        // TOK_PERCENT_EQUALS,
        case '%':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_PERCENT_EQUALS);
            else addSimpleToken(&lexer, TOK_PERCENT);
            break;
        // TOK_BANG,
        // TOK_BANG_EQUALS,
        case '!':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_BANG_EQUALS);
            else addSimpleToken(&lexer, TOK_BANG);
            break;
        // TOK_LESS,
        // TOK_LESS_EQUALS,
        // TOK_LESS_LESS,
        // TOK_LESS_LESS_EQUALS,
        case '<':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_LESS_EQUALS);
            else if (match(&lexer, '<')) {
                if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_LESS_LESS_EQUALS);
                else addSimpleToken(&lexer, TOK_LESS_LESS);
            } else addSimpleToken(&lexer, TOK_LESS);
            break;
        // TOK_GREATER,
        // TOK_GREATER_EQUALS,
        // TOK_GREATER_GREATER,
        // TOK_GREATER_GREATER_EQUALS,
        case '>':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_GREATER_EQUALS);
            else if (match(&lexer, '>')) {
                if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_GREATER_GREATER_EQUALS);
                else addSimpleToken(&lexer, TOK_GREATER_GREATER);
            } else addSimpleToken(&lexer, TOK_GREATER);
            break;
        // TOK_AMPERSAND,
        // TOK_AMPERSAND_AMPERSAND,
        // TOK_AMPERSAND_EQUALS,
        case '&':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_AMPERSAND_EQUALS);
            else if (match(&lexer, '&')) addSimpleToken(&lexer, TOK_AMPERSAND_AMPERSAND);
            else addSimpleToken(&lexer, TOK_AMPERSAND);
            break;
        // TOK_PIPE,
        // TOK_PIPE_PIPE,
        // TOK_PIPE_EQUALS,
        case '|':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_PIPE_EQUALS);
            else if (match(&lexer, '|')) addSimpleToken(&lexer, TOK_PIPE_PIPE);
            else addSimpleToken(&lexer, TOK_PIPE);
            break;
        // TOK_CARET,
        // TOK_CARET_EQUALS,
        case '^':
            if (match(&lexer, '=')) addSimpleToken(&lexer, TOK_CARET_EQUALS);
            else addSimpleToken(&lexer, TOK_CARET);
            break;
        // TOK_TILDE,
        case '~':
            addSimpleToken(&lexer, TOK_TILDE);
            break;
        // TOK_LEFT_PAREN,
        case '(':
            addSimpleToken(&lexer, TOK_LEFT_PAREN);
            break;
        // TOK_RIGHT_PAREN,
        case ')':
            addSimpleToken(&lexer, TOK_RIGHT_PAREN);
            break;
        // TOK_LEFT_BRACE,
        case '{':
            addSimpleToken(&lexer, TOK_LEFT_BRACE);
            break;
        // TOK_RIGHT_BRACE,
        case '}':
            addSimpleToken(&lexer, TOK_RIGHT_BRACE);
            break;
        // TOK_LEFT_BRACKET,
        case '[':
            addSimpleToken(&lexer, TOK_LEFT_BRACKET);
            break;
        // TOK_RIGHT_BRACKET,
        case ']':
            addSimpleToken(&lexer, TOK_RIGHT_BRACKET);
            break;
        // TOK_COMMA,
        case ',':
            addSimpleToken(&lexer, TOK_COMMA);
            break;
        // TOK_DOT,
        // TOK_ELLIPSIS,
        case '.':
            if (peek(&lexer) == '.' && peekAhead(&lexer, 1) == '.') {
                advance(&lexer);
                advance(&lexer);
                addSimpleToken(&lexer, TOK_ELLIPSIS);
            }
            else addSimpleToken(&lexer, TOK_DOT);
            break;
        // TOK_SEMICOLON,
        case ';':
            addSimpleToken(&lexer, TOK_SEMICOLON);
            break;
        // TOK_COLON,
        // TOK_COLON_COLON,
        case ':':
            addSimpleToken(&lexer, TOK_COLON_COLON);
            break;
        // TOK_QUESTION_MARK,
        case '?':
            addSimpleToken(&lexer, TOK_QUESTION_MARK);
            break;
        // TOK_AT
        case '@':
            addSimpleToken(&lexer, TOK_AT);
            break;

        case '\0':
            addSimpleToken(&lexer, TOK_EOF);
            break;

        default:
            LIST_appendSingle(lexer.tokens, makeUnknown(c, lexer.line, lexer.col));
            break;
        }
    }

    free(lexer.input.arr);
    return NO_ERR;
}

void freeTokensList(TokensList *tokens) {
    for (usize i = 0; i < tokens->len; i++) {
        Token *tok = &tokens->arr[i];
        switch (tok->type) {
        case TOK_IDENTIFIER:
            free((void *)tok->as.identifier.arr);
            break;
        case TOK_STRING_LITERAL:
            free((void *)tok->as.stringLiteral.arr);
            break;
        default:
            break;
        }
    }
    free(tokens->arr);
    tokens->arr = NULL;
    tokens->len = 0;
    tokens->cap = 0;
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
    case TOK_INTEGER_LITERAL:
        printf(": (%lu)\n", token.as.integerLiteral);
        return;
    case TOK_FLOAT_LITERAL:
        printf(": (%f)\n", token.as.floatLiteral);
        return;
    case TOK_UNKNOWN:
        printf(": (\\x%02d)", token.as.unknown);
        if (' ' <= token.as.unknown && token.as.unknown <= '~') printf(" : \'%c\'\n", token.as.unknown);
        else printf("\n");
        break;
    default:
        printf("\n");
        return;
    }
}
