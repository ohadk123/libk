#ifndef INCLUDE_INCLUDE_LEXER_H_
#define INCLUDE_INCLUDE_LEXER_H_

#include "Token.h"

ErrCode scanFile(TokensList *dest, String path);
void freeTokensList(TokensList *tokens);
void printToken(Token token);

#endif // INCLUDE_INCLUDE_LEXER_H_
