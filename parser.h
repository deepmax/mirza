#ifndef PARSER_H
#define PARSER_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void parser_load(const char_t* filename);
void parser_free();
void parser_start();

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
