#include "parser.h"
#include "vm.h"
#include <stdio.h>

void test_parser()
{
    printf("---------\n");

    parser_load("examples/code2.lm");
    parser_start();
    parser_free();

    printf("\n---------\n");
}

void test_vm()
{
    vm_init(1024, 1024);

    EMIT(ICONST, NUM32(0xFF));
    EMIT(CALL, NUM16(0x9));
    EMIT(HALT);

    EMIT(PROC, NUM16(1), NUM16(0));
    EMIT(ILOAD, NUM16(0));
    EMIT(IPRINT);

    EMIT(ICONST_0);
    EMIT(RET);

    vm_dasm();
    vm_exec();

    vm_free();
}

int main()
{
    test_parser();
    // test_vm();
}
