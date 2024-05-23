#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include "token.h"

#ifdef __cplusplus
extern "C"
{
#endif

void lexer_init(const char_t* filename);
void lexer_free();
token_t lexer_next();
uint32_t lexer_row();
uint32_t lexer_col();

#ifdef __cplusplus
}
#endif

#endif /* LEXER_H */
