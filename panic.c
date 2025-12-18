#include "panic.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

void panic(const char* msg)
{
    fprintf(stderr, "%s : %d %d", msg, lexer_row() + 1, lexer_col() + 1);
    exit(0);
}
