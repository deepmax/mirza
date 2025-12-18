#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include "token.h"

#ifdef __cplusplus
extern "C"
{
#endif

void lexer_init_file(const char* filename);
void lexer_init_stdin();
void lexer_free();
token_t lexer_next();
uint32_t lexer_row();
uint32_t lexer_col();

#ifdef __cplusplus
}
#endif

#endif /* LEXER_H */
