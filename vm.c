#include "vm.h"
#include "buffer.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct
{
    uint32_t ip;          // Points the index of current machine instruction to execute: program[ip] or *(program + ip)
    uint32_t sp;          // Points the top element of the machine stack: stack[sp]
    uint32_t bp;          // Base index
    value_t* stack;
    size_t stack_size;
    buffer_t program;
    buffer_t data;
    struct {
        uint8_t halt: 1;
    } flags;
} vm_t;

static vm_t vm;

const opcode_t OPCODES[] = {
    {NOP, 0, "nop"},
    {IINC, 0, "iinc"},
    {IDEC, 0, "idec"},
    {INEG, 0, "ineg"},
    {IABS, 0, "iabs"},
    {INOT, 0, "inot"},
    {IADD, 0, "iadd"},
    {ISUB, 0, "isub"},
    {IDIV, 0, "idiv"},
    {IMOD, 0, "imod"},
    {IMUL, 0, "imul"},
    {IAND, 0, "iand"},
    {IXOR, 0, "ixor"},
    {IOR, 0, "ior"},
    {ISHL, 0, "ishl"},
    {ISHR, 0, "ishr"},
    {IGT, 0, "igt"},
    {ILT, 0, "ilt"},
    {IGE, 0, "ige"},
    {ILE, 0, "ile"},
    {IEQ, 0, "ieq"},
    {INQ, 0, "inq"},
    {ILOAD, 2, "iload"},
    {ISTORE, 2, "istore"},
    {ILOADG, 2, "iloadg"},
    {ISTOREG, 2, "istoreg"},
    {ICONST, 4, "iconst"},
    {ICONST_0, 0, "iconst_0"},
    {ICONST_1, 0, "iconst_1"},
    {IPRINT, 0, "iprint"},
    {ITOR, 0, "itor"},
    {RINC, 0, "rinc"},
    {RDEC, 0, "rdec"},
    {RNEG, 0, "rneg"},
    {RABS, 0, "rabs"},
    {RADD, 0, "radd"},
    {RSUB, 0, "rsub"},
    {RDIV, 0, "rdiv"},
    {RMOD, 0, "rmod"},
    {RMUL, 0, "rmul"},
    {RPOW, 0, "rpow"},
    {RSQRT, 0, "rsqrt"},
    {REXP, 0, "rexp"},
    {RSIN, 0, "rsin"},
    {RCOS, 0, "rcos"},
    {RTAN, 0, "rtan"},
    {RASIN, 0, "rasin"},
    {RACOS, 0, "racos"},
    {RATAN2, 0, "ratan2"},
    {RLOG, 0, "rlog"},
    {RLOG10, 0, "rlog10"},
    {RLOG2, 0, "rlog2"},
    {RCEIL, 0, "rceil"},
    {RFLOOR, 0, "rfloor"},
    {RROUND, 0, "rround"},
    {RGT, 0, "rgt"},
    {RLT, 0, "rlt"},
    {RGE, 0, "rge"},
    {RLE, 0, "rle"},
    {REQ, 0, "req"},
    {RNQ, 0, "rnq"},
    {RLOAD, 2, "rload"},
    {RSTORE, 2, "rstore"},
    {RLOADG, 2, "rloadg"},
    {RSTOREG, 2, "rstoreg"},
    {RCONST, 8, "rconst"},
    {RCONST_0, 0, "rconst_0"},
    {RCONST_1, 0, "rconst_1"},
    {RCONST_PI, 0, "rconst_pi"},
    {RPRINT, 0, "rprint"},
    {RTOI, 0, "rtoi"},
    {ALOAD, 2, "aload"},
    {ASTORE, 2, "astore"},
    {ALOADG, 2, "aloadg"},
    {ASTOREG, 2, "astoreg"},
    {ACONST, 2, "aconst"},
    {APRINT, 0, "aprint"},
    {NPRINT, 0, "nprint"},
    {DUP, 0, "dup"},
    {DROP, 0, "drop"},
    {ALLC, 2, "allc"},
    {SWAP, 0, "swap"},
    {PROC, 4, "proc"},
    {CALL, 2, "call"},
    {RET, 0, "ret"},
    {JNZ, 2, "jnz"},
    {JEZ, 2, "jez"},
    {JMP, 2, "jmp"},
    {HALT, 0, "halt"},
};

void vm_init(size_t stack_size, size_t code_size)
{
    buffer_init(&vm.data, 0);
    buffer_init(&vm.program, code_size);
    vm.stack = malloc(sizeof (type_t) * stack_size);
    vm.stack_size = stack_size;
    vm.ip = 0;
    vm.sp = 0;
    vm.bp = 0;
    vm.flags.halt = 0;
}

void vm_free()
{
    free(vm.stack);
    buffer_free(&vm.data);
    buffer_free(&vm.program);
}

void exec_opcode(uint8_t* opcode)
{
    switch (*opcode)
    {
    case HALT:
    {
        vm.flags.halt = 1;
        ++vm.ip;
        break;
    }
    case NOP:
    {
        ++vm.ip;
        break;
    }
    case DUP:
    {
        vm.stack[vm.sp + 1] = vm.stack[vm.sp];
        ++vm.sp;
        ++vm.ip;
        break;
    }
    case SWAP:
    {
        value_t tmp = vm.stack[vm.sp];
        vm.stack[vm.sp] = vm.stack[vm.sp - 1];
        vm.stack[vm.sp - 1] = tmp;
        ++vm.ip;
        break;
    }
    case DROP:
    {
        vm.sp -= *((uint16_t*) (opcode + 1));
        vm.ip += 3;
        break;
    }
    case ALLC:
    {
        vm.sp += *((uint16_t*) (opcode + 1));
        vm.ip += 3;
        break;
    }
    case PROC:
    {
        uint16_t args = *((uint16_t*) (opcode + 1));
        uint16_t vars = *((uint16_t*) (opcode + 3));
        uint32_t _bp = vm.stack[vm.sp].as_uint32;
        uint32_t _ip = vm.stack[vm.sp - 1].as_uint32;
        vm.stack[vm.sp].as_uint32 = 0;
        vm.stack[vm.sp - 1].as_uint32 = 0;
        vm.sp += vars - 2;
        vm.stack[++vm.sp].as_uint32 = _ip;
        vm.stack[++vm.sp].as_uint32 = _bp;
        vm.stack[++vm.sp].as_uint32 = args + vars;
        vm.bp = vm.sp - (args + vars + 2);
        vm.ip += 5;
        break;
    }
    case CALL:
    {
        vm.stack[++vm.sp].as_uint32 = vm.ip + 3;
        vm.stack[++vm.sp].as_uint32 = vm.bp;
        vm.ip = *((uint16_t*) (opcode + 1));
        break;
    }
    case RET:
    {
        value_t retv = vm.stack[vm.sp--];
        uint32_t drops = vm.stack[vm.sp--].as_uint32;
        uint32_t _bp = vm.stack[vm.sp--].as_uint32;
        uint32_t _ip = vm.stack[vm.sp--].as_uint32;
        vm.sp -= drops;
        vm.stack[++vm.sp] = retv;
        vm.ip = (uint16_t) _ip;
        vm.bp = (uint16_t) _bp;
        break;
    }
    case JMP:
    {
        vm.ip = *((uint16_t*) (opcode + 1));
        break;
    }
    case JEZ:
    {
        if (vm.stack[vm.sp].as_int32 == 0)
            vm.ip = *((uint16_t*) (opcode + 1));
        else
            vm.ip += 3;
        --vm.sp;
        break;
    }
    case JNZ:
    {
        if (vm.stack[vm.sp].as_int32 != 0)
            vm.ip = *((uint16_t*) (opcode + 1));
        else
            vm.ip += 3;
        --vm.sp;
        break;
    }
    case ILOAD:
    {
        vm.stack[vm.sp + 1].as_int32 = vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_int32;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case ISTORE:
    {
        vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_int32 = vm.stack[vm.sp].as_int32;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case ILOADG:
    {
        vm.stack[vm.sp + 1].as_int32 = vm.stack[*((uint16_t*) (opcode + 1))].as_int32;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case ISTOREG:
    {
        vm.stack[*((uint16_t*) (opcode + 1))].as_int32 = vm.stack[vm.sp].as_int32;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case IINC:
    {
        vm.stack[vm.sp].as_int32++;
        ++vm.ip;
        break;
    }
    case IDEC:
    {
        vm.stack[vm.sp].as_int32--;
        ++vm.ip;
        break;
    }
    case INEG:
    {
        vm.stack[vm.sp].as_int32 *= -1;
        ++vm.ip;
        break;
    }
    case IABS:
    {
        if (vm.stack[vm.sp].as_int32 < 0)
            vm.stack[vm.sp].as_int32 *= -1;
        ++vm.ip;
        break;
    }
    case INOT:
    {
        vm.stack[vm.sp].as_int32 = ~vm.stack[vm.sp].as_int32;
        ++vm.ip;
        break;
    }
    case IADD:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 + vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ISUB:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 - vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IMUL:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 * vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IDIV:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 / vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IMOD:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 % vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IAND:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 && vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IXOR:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 ^ vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IOR:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 || vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ISHL:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 << vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ISHR:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 >> vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IGT:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 > vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ILT:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 < vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IGE:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 >= vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ILE:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 <= vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case IEQ:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 == vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case INQ:
    {
        vm.stack[vm.sp - 1].as_int32 = vm.stack[vm.sp - 1].as_int32 != vm.stack[vm.sp].as_int32;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ICONST:
    {
        vm.stack[++vm.sp].as_int32 = *((int32_t*) (opcode + 1));
        vm.ip += 5;
        break;
    }
    case ICONST_0:
    {
        vm.stack[++vm.sp].as_int32 = 0;
        ++vm.ip;
        break;
    }
    case ICONST_1:
    {
        vm.stack[++vm.sp].as_int32 = 1;
        ++vm.ip;
        break;
    }
    case IPRINT:
    {
        printf("%d", vm.stack[vm.sp].as_int32);
        fflush(stdout);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case ITOR:
    {
        vm.stack[vm.sp].as_real = (real_t) vm.stack[vm.sp].as_int32;
        ++vm.ip;
        break;
    }
    case RLOAD:
    {
        vm.stack[vm.sp + 1].as_real = vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_real;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case RSTORE:
    {
        vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_real = vm.stack[vm.sp].as_real;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case RLOADG:
    {
        vm.stack[vm.sp + 1].as_real = vm.stack[*((uint16_t*) (opcode + 1))].as_real;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case RSTOREG:
    {
        vm.stack[*((uint16_t*) (opcode + 1))].as_real = vm.stack[vm.sp].as_real;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case RINC:
    {
        vm.stack[vm.sp].as_real++;
        ++vm.ip;
        break;
    }
    case RDEC:
    {
        vm.stack[vm.sp].as_real--;
        ++vm.ip;
        break;
    }
    case RNEG:
    {
        vm.stack[vm.sp].as_real *= -1;
        ++vm.ip;
        break;
    }
    case RABS:
    {
        vm.stack[vm.sp].as_real = fabs(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RADD:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real + vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RSUB:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real - vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RMUL:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real * vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RDIV:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real / vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RMOD:
    {
        vm.stack[vm.sp - 1].as_real = fmod(vm.stack[vm.sp - 1].as_real, vm.stack[vm.sp].as_real);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RPOW:
    {
        vm.stack[vm.sp - 1].as_real = pow(vm.stack[vm.sp - 1].as_real, vm.stack[vm.sp].as_real);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RSQRT:
    {
        vm.stack[vm.sp].as_real = sqrt(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case REXP:
    {
        vm.stack[vm.sp].as_real = exp(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RSIN:
    {
        vm.stack[vm.sp].as_real = sin(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RCOS:
    {
        vm.stack[vm.sp].as_real = cos(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RTAN:
    {
        vm.stack[vm.sp].as_real = tan(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RASIN:
    {
        vm.stack[vm.sp].as_real = asin(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RACOS:
    {
        vm.stack[vm.sp].as_real = acos(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RATAN2:
    {
        vm.stack[vm.sp - 1].as_real = atan2(vm.stack[vm.sp - 1].as_real, vm.stack[vm.sp].as_real);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RLOG:
    {
        vm.stack[vm.sp].as_real = log(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RLOG10:
    {
        vm.stack[vm.sp].as_real = log10(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RLOG2:
    {
        vm.stack[vm.sp].as_real = log2(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RCEIL:
    {
        vm.stack[vm.sp].as_real = ceil(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RFLOOR:
    {
        vm.stack[vm.sp].as_real = floor(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RROUND:
    {
        vm.stack[vm.sp].as_real = round(vm.stack[vm.sp].as_real);
        ++vm.ip;
        break;
    }
    case RGT:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real > vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RLT:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real < vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RGE:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real >= vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RLE:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real <= vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case REQ:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real == vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RNQ:
    {
        vm.stack[vm.sp - 1].as_real = vm.stack[vm.sp - 1].as_real != vm.stack[vm.sp].as_real;
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RCONST:
    {
        vm.stack[++vm.sp].as_uint64 = *((uint64_t*) (opcode + 1));
        vm.ip += 9;
        break;
    }
    case RCONST_0:
    {
        vm.stack[++vm.sp].as_real = 0.0;
        ++vm.ip;
        break;
    }
    case RCONST_1:
    {
        vm.stack[++vm.sp].as_real = 1.0;
        ++vm.ip;
        break;
    }
    case RCONST_PI:
    {
        vm.stack[++vm.sp].as_real = 3.14159265358979323846;
        ++vm.ip;
        break;
    }
    case RPRINT:
    {
        printf("%f", vm.stack[vm.sp].as_real);
        fflush(stdout);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case RTOI:
    {
        vm.stack[vm.sp].as_int32 = (int32_t) vm.stack[vm.sp].as_real;
        ++vm.ip;
        break;
    }
    case ALOAD:
    {
        vm.stack[vm.sp + 1].as_uint16 = vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_uint16;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case ASTORE:
    {
        vm.stack[vm.bp + *((uint16_t*) (opcode + 1))].as_uint16 = vm.stack[vm.sp].as_uint16;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case ALOADG:
    {
        vm.stack[vm.sp + 1].as_uint16 = vm.stack[*((uint16_t*) (opcode + 1))].as_uint16;
        ++vm.sp;
        vm.ip += 3;
        break;
    }
    case ASTOREG:
    {
        vm.stack[*((uint16_t*) (opcode + 1))].as_uint16 = vm.stack[vm.sp].as_uint16;
        --vm.sp;
        vm.ip += 3;
        break;
    }
    case ACONST:
    {
        vm.stack[++vm.sp].as_int16 = *((uint16_t*) (opcode + 1));
        vm.ip += 3;
        break;
    }
    case APRINT:
    {
        printf("%s", &vm.data.data[vm.stack[vm.sp].as_uint16]);
        fflush(stdout);
        --vm.sp;
        ++vm.ip;
        break;
    }
    case NPRINT:
    {
        printf("\n");
        fflush(stdout);
        ++vm.ip;
        break;
    }
    default:
        printf("BAD OPCODE [%d : %d]\n", *opcode, vm.ip);
        exit(0);
        break;
    }
}

void vm_exec()
{
    while (!vm.flags.halt)
    {
        exec_opcode(vm.program.data + vm.ip);
    }
}

void vm_dump()
{
    printf("-- begin --\n");
    printf("ip: %du  sp: %du bp: %du", vm.ip, vm.sp, vm.bp);
    printf("[ ");
    for (int i = vm.sp; i >= 0; i--)
        printf("%lu ", vm.stack[i].as_uint64);
    printf("]\n");
    printf("-- end   --\n");
}

void vm_dasm()
{
    for (size_t i = 0; i < vm.program.used; i++)
    {
        opcode_t opcode = OPCODES[vm.program.data[i]];

        printf("%lx\t %s", i, opcode.name);

        if (opcode.arg_size == 0)
        {
            printf("\n");
        }
        else
        {
            for (int a = 0; a < opcode.arg_size; a++)
                printf(" 0x%x", (vm.program.data[i + a + 1] & 0xFF));
            printf("\n");
        }

        i += opcode.arg_size;
    }
}

void vm_code_emit(uint8_t* bytes, size_t len)
{
    buffer_adds(&vm.program, bytes, len);
}

void vm_code_set(size_t index, uint8_t* bytes, size_t len)
{
    buffer_sets(&vm.program, index, bytes, len);
}

size_t vm_code_addr()
{
    return buffer_size(&vm.program);
}

void vm_data_emit(uint8_t* bytes, size_t len)
{
    buffer_adds(&vm.data, bytes, len);
}

size_t vm_data_addr()
{
    return buffer_size(&vm.data);
}

void vm_save(char* name)
{
    // this must have data ...
    FILE* file = fopen(name, "w");
    fwrite(vm.program.data, vm.program.used, 1, file);
    fclose(file);
}

void vm_load(char* name)
{
}
