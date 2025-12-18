#ifndef PARSER_H
#define PARSER_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void parser_load(const char* filename);
void parser_stdin();
void parser_free();
void parser_start(bool execute, bool dasm);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
