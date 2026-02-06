#include "Lexer.h"
#include "String.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

static inline Bool isHex(u8 c) {
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

static inline u8 hexToVal(u8 c) {
    if ('0' <= c && c <= '9') return c - '0' + 0x0;
    if ('a' <= c && c <= 'f') return c - 'a' + 0xa;
    if ('A' <= c && c <= 'F') return c - 'A' + 0xA;

    assert (FALSE && "Called hexToVal with non hex character!");
}

static inline Bool isOct(u8 c) {
    return '0' <= c && c <= '7';
}

static inline u8 octToVal(u8 c) {
    if ('0' <= c && c <= '7') return c - '0';

    assert (FALSE && "Called octToVal with non octal character!");
}

typedef struct {
    String input;
    usize line;
    usize col;
    usize index;
    TokensList *tokens;
    Bool hasErros;
} Lexer;

static inline void addToken(Lexer *l, Token token) {
    LIST_appendSingle(l->tokens, token);
    if (token.type == TOK_ERROR) l->hasErros = TRUE;
}

static void addSimpleToken(Lexer *l, TokenType type) {
    Token t = makeSimple(type, l->line, l->col);
    addToken(l, t);
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

static void makeIdentifier(Lexer *l) {
    usize start = l->index - 1;
    usize col = l->col;

    while (isalnum(peek(l)) || peek(l) == '_') advance(l);
    usize end = l->index;

    String identString = {0};
    ErrCode err;
    if ((err = joinStringSlice(&identString, &l->input, start, end)) != NO_ERR) exit(err);

    Token identToken = makeIdentifierToken(identString,l->line, col);
    addToken(l, identToken);
}

static void makeNumber(Lexer *l) {
    Bool isFloat = FALSE;
    usize start = l->index - 1;
    usize col = l->col;

    while (isdigit(peek(l)) || (peek(l) == '.' && !isFloat)) {
        if (l->input.arr[l->index-1] == '.') isFloat = TRUE;
        advance(l);
    }
    usize end = l->index;

    String numberString = {0};
    ErrCode err;
    if ((err = joinStringSlice(&numberString, &l->input, start, end)) != NO_ERR) exit(err);
    joinByte(&numberString, '\0');

    if (isFloat) {
        f64 fval = atof((cstr) numberString.arr);
        Token floatToken = makeFloatLiteralToken(fval, l->line, col);
        addToken(l, floatToken);
    } else {
        u64 ival = atoll((cstr) numberString.arr);
        Token floatToken = makeIntegerLiteralToken(ival, l->line, col);
        addToken(l, floatToken);
    }
}

static u8 consumeEscapeChar(Lexer *l) {
    assert(l->input.arr[l->index-1] == '\\' && "Called consumeEscapeChar without being on '\\'");
    u64 val = 0;

    u8 c = advance(l);
    switch (c) {
        case 'n':  val = '\n'; break;
        case 't':  val = '\t'; break;
        case 'v':  val = '\v'; break;
        case 'b':  val = '\b'; break;
        case 'r':  val = '\r'; break;
        case 'f':  val = '\f'; break;
        case 'a':  val = '\a'; break;
        case '\\': val = '\\'; break;
        case '?':  val = '\?'; break;
        case '\'': val = '\''; break;
        case '\"': val = '\"'; break;
        case 'x':
            while (isHex(peek(l))) {
                u8 c = advance(l);
                val *= 16;
                val += hexToVal(c);
            }
            break;
        case '0':
            while (isOct(peek(l))) {
                u8 c = advance(l);
                val *= 8;
                val += octToVal(c);
            }
            break;
    }

    if (val > U8_MAX) {
        String errString = {0};
        joinCString(&errString, "Escape squence out of u8 range");
        Token token = makeErrorToken(errString, l->line, l->col);
        addToken(l, token);
        return -1;
    }
    return (u8)val;
}

static void makeString(Lexer *l) {
    String stringString = {0};
    usize col = l->col;
    Token token = {0};

    while (!isAtEnd(l) && peek(l) != '\n') {
        if (peek(l) == '\"') {
            advance(l);
            goto terminated;
        }

        u8 c = advance(l);
        if (c == '\\') c = consumeEscapeChar(l);
        joinByte(&stringString, c);
    }
    String errString = {0};
    joinCString(&errString, "Unterminated String Literal!");
    token = makeErrorToken(errString, l->line, col);
    addToken(l, token);
    return;

terminated:
    token = makeStringLiteralToken(stringString, l->line, col);
    addToken(l, token);
}

static void makeChar(Lexer *l) {
    usize col = l->col;
    u8 c = advance(l);

    if (c == '\\') c = consumeEscapeChar(l);
    if (peek(l) != '\'') {
        String errString = {0};
        joinCString(&errString, "Unterminated Char Literal!");
        Token token = makeErrorToken(errString, l->line, col);
        addToken(l, token);
        return;
    }
    advance(l); // Consume temrminating '

    Token token = makeCharLiteralToken(c, l->line, col);
    addToken(l, token);
    return;
}

#define ADD_SIMPLE(type) addSimpleToken(&lexer, type)

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

        if (isalpha(c) || c == '_') {
            makeIdentifier(&lexer);
            continue;
        }

        if (isdigit(c) || (c == '.' && isdigit(peek(&lexer)))) {
            makeNumber(&lexer);
            continue;
        }

        if (c == '\"') {
            makeString(&lexer);
            continue;
        }

        if (c == '\'') {
            makeChar(&lexer);
            continue;
        }

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
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_EQUALS_EQUALS);
            else ADD_SIMPLE(TOK_EQUALS);
            break;
        // TOK_PLUS,
        // TOK_PLUS_PLUS,
        // TOK_PLUS_EQUALS,
        case '+':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_PLUS_EQUALS);
            else if (match(&lexer, '+')) ADD_SIMPLE(TOK_PLUS_PLUS);
            else ADD_SIMPLE(TOK_PLUS);
            break;
        // TOK_MINUS,
        // TOK_MINUS_MINUS,
        // TOK_MINUS_EQUALS,
        // TOK_MINUS_GREATER,
        case '-':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_MINUS_EQUALS);
            else if (match(&lexer, '-')) ADD_SIMPLE(TOK_MINUS_MINUS);
            else if (match(&lexer, '>')) ADD_SIMPLE(TOK_MINUS_GREATER);
            else ADD_SIMPLE(TOK_MINUS);
            break;
        // TOK_STAR,
        // TOK_STAR_EQUALS,
        case '*':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_STAR_EQUALS);
            else ADD_SIMPLE(TOK_STAR);
            break;
        // TOK_SLASH,
        // TOK_SLASH_EQUALS,
        case '/':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_SLASH_EQUALS);
            else ADD_SIMPLE(TOK_SLASH);
            break;
        // TOK_PERCENT,
        // TOK_PERCENT_EQUALS,
        case '%':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_PERCENT_EQUALS);
            else ADD_SIMPLE(TOK_PERCENT);
            break;
        // TOK_BANG,
        // TOK_BANG_EQUALS,
        case '!':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_BANG_EQUALS);
            else ADD_SIMPLE(TOK_BANG);
            break;
        // TOK_LESS,
        // TOK_LESS_EQUALS,
        // TOK_LESS_LESS,
        // TOK_LESS_LESS_EQUALS,
        case '<':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_LESS_EQUALS);
            else if (match(&lexer, '<')) {
                if (match(&lexer, '=')) ADD_SIMPLE(TOK_LESS_LESS_EQUALS);
                else ADD_SIMPLE(TOK_LESS_LESS);
            } else ADD_SIMPLE(TOK_LESS);
            break;
        // TOK_GREATER,
        // TOK_GREATER_EQUALS,
        // TOK_GREATER_GREATER,
        // TOK_GREATER_GREATER_EQUALS,
        case '>':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_GREATER_EQUALS);
            else if (match(&lexer, '>')) {
                if (match(&lexer, '=')) ADD_SIMPLE(TOK_GREATER_GREATER_EQUALS);
                else ADD_SIMPLE(TOK_GREATER_GREATER);
            } else ADD_SIMPLE(TOK_GREATER);
            break;
        // TOK_AMPERSAND,
        // TOK_AMPERSAND_AMPERSAND,
        // TOK_AMPERSAND_EQUALS,
        case '&':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_AMPERSAND_EQUALS);
            else if (match(&lexer, '&')) ADD_SIMPLE(TOK_AMPERSAND_AMPERSAND);
            else ADD_SIMPLE(TOK_AMPERSAND);
            break;
        // TOK_PIPE,
        // TOK_PIPE_PIPE,
        // TOK_PIPE_EQUALS,
        case '|':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_PIPE_EQUALS);
            else if (match(&lexer, '|')) ADD_SIMPLE(TOK_PIPE_PIPE);
            else ADD_SIMPLE(TOK_PIPE);
            break;
        // TOK_CARET,
        // TOK_CARET_EQUALS,
        case '^':
            if (match(&lexer, '=')) ADD_SIMPLE(TOK_CARET_EQUALS);
            else ADD_SIMPLE(TOK_CARET);
            break;
        // TOK_TILDE,
        case '~':
            ADD_SIMPLE(TOK_TILDE);
            break;
        // TOK_LEFT_PAREN,
        case '(':
            ADD_SIMPLE(TOK_LEFT_PAREN);
            break;
        // TOK_RIGHT_PAREN,
        case ')':
            ADD_SIMPLE(TOK_RIGHT_PAREN);
            break;
        // TOK_LEFT_BRACE,
        case '{':
            ADD_SIMPLE(TOK_LEFT_BRACE);
            break;
        // TOK_RIGHT_BRACE,
        case '}':
            ADD_SIMPLE(TOK_RIGHT_BRACE);
            break;
        // TOK_LEFT_BRACKET,
        case '[':
            ADD_SIMPLE(TOK_LEFT_BRACKET);
            break;
        // TOK_RIGHT_BRACKET,
        case ']':
            ADD_SIMPLE(TOK_RIGHT_BRACKET);
            break;
        // TOK_COMMA,
        case ',':
            ADD_SIMPLE(TOK_COMMA);
            break;
        // TOK_DOT,
        // TOK_ELLIPSIS,
        case '.':
            if (peek(&lexer) == '.' && peekAhead(&lexer, 1) == '.') {
                advance(&lexer);
                advance(&lexer);
                ADD_SIMPLE(TOK_ELLIPSIS);
            }
            else ADD_SIMPLE(TOK_DOT);
            break;
        // TOK_SEMICOLON,
        case ';':
            ADD_SIMPLE(TOK_SEMICOLON);
            break;
        // TOK_COLON,
        // TOK_COLON_COLON,
        case ':':
            if (match(&lexer, ':')) ADD_SIMPLE(TOK_COLON_COLON);
            else ADD_SIMPLE(TOK_COLON);
            break;
        // TOK_QUESTION_MARK,
        case '?':
            ADD_SIMPLE(TOK_QUESTION_MARK);
            break;
        // TOK_AT
        case '@':
            ADD_SIMPLE(TOK_AT);
            break;

        case '\0':
            ADD_SIMPLE(TOK_EOF);
            break;

        default:
            LIST_appendSingle(lexer.tokens, makeUnknown(c, lexer.line, lexer.col));
            break;
        }
    }

    free(lexer.input.arr);
    return lexer.hasErros ? LEXING_ERR : NO_ERR;
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
