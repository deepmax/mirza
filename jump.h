#ifndef JUMP_H
#define JUMP_H

#include "types.h"
#include "vector.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    vector_t* jumps;
    uint16_t label;
} jump_t;


jump_t* jump_new();
void jump_free(jump_t* jump);
void jump_to(jump_t* jump);
void jump_label(jump_t* jump);
void jump_fix(jump_t* jump);

#ifdef __cplusplus
}
#endif

#endif /* JUMP_H */
