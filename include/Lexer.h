#ifndef INCLUDE_INCLUDE_LEXER_H_
#define INCLUDE_INCLUDE_LEXER_H_

#include "private/Token.h"

ErrCode scanFile(TokensList *dest, cstr path);
void freeTokensList(TokensList *tokens);
void printToken(Token token);

#endif // INCLUDE_INCLUDE_LEXER_H_
