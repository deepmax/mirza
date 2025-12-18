#ifndef PANIC_H
#define PANIC_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

void panic(const char* msg);

#ifdef __cplusplus
}
#endif

#endif /* PANIC_H */
