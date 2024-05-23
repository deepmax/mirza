#ifndef VM_H
#define VM_H

#include "types.h"

#define EMIT(...) do{byte_t b[] = { __VA_ARGS__ }; vm_code_emit(b, sizeof(b) / sizeof(b[0]));}while(0)
#define CODE(i, ...) do{byte_t b[] = { __VA_ARGS__ }; vm_code_set(i, b, sizeof(b) / sizeof(b[0]));}while(0)
//#define DATA(...) do{byte_t b[] = { __VA_ARGS__ }; vm_data_emit(b, sizeof(b) / sizeof(b[0]));}while(0)

enum
{
    NOP,
    // int32 --> todo: convert int64, or have them all ?!
    IINC,
    IDEC,
    INEG,
    IABS,
    INOT,
    IADD,
    ISUB,
    IDIV,
    IMOD,
    IMUL,
    IAND,
    IXOR,
    IOR,
    ISHL,
    ISHR,
    IGT,
    ILT,
    IGE,
    ILE,
    IEQ,
    INQ,
    ILOAD,
    ISTORE,
    ILOADG,
    ISTOREG,
    ICONST,
    ICONST_0,
    ICONST_1,
    IPRINT,
    ITOR,

    // real
    RINC,
    RDEC,
    RNEG,
    RABS,
    RADD,
    RSUB,
    RDIV,
    RMOD,
    RMUL,
    RPOW,
    RSQRT,
    REXP,
    RSIN,
    RCOS,
    RTAN,
    RASIN,
    RACOS,
    RATAN2,
    RLOG,
    RLOG10,
    RLOG2,
    RCEIL,
    RFLOOR,
    RROUND,
    RGT, // isgreater
    RLT, // isless
    RGE, // isgreaterequal
    RLE, // islessequal
    REQ,
    RNQ, // islessgreater ? isinf ? isnan ?
    RLOAD,
    RSTORE,
    RLOADG,
    RSTOREG,
    RCONST,
    RCONST_0,
    RCONST_1,
    RCONST_PI,
    RPRINT,
    RTOI,

    ALOAD,
    ASTORE,
    ALOADG,
    ASTOREG,
    ACONST,
    APRINT,

    // XLOAD
    // XLOADG
    // XSTORE
    // XSTOREG

    NPRINT,

    DUP,
    DROP,
    ALLC,
    SWAP,

    PROC,
    CALL,
    RET,

    JNZ,
    JEZ,
    JMP,

    HALT,
};

#define NUM64(X) \
    (X & 0xFF), \
    ((X >> 8) & 0xFF), \
    ((X >> 16) & 0xFF), \
    ((X >> 24) & 0xFF), \
    ((X >> 32) & 0xFF), \
    ((X >> 40) & 0xFF), \
    ((X >> 48) & 0xFF), \
    ((X >> 56) & 0xFF)

#define NUM32(X) \
    (X & 0xFF), \
    ((X >> 8) & 0xFF), \
    ((X >> 16) & 0xFF), \
    ((X >> 24) & 0xFF)

#define NUM16(X) \
    (X & 0xFF), \
    ((X >> 8) & 0xFF)

#define NUM8(X) \
    (X & 0xFF)

typedef struct
{
    const uint8_t code;
    const uint8_t arg_size;
    const char_t name[32];
} opcode_t;

extern const opcode_t OPCODES[];

void vm_init(size_t stack_size, size_t program_size);
void vm_free();
void vm_exec();
void vm_dump();
void vm_dasm();
void vm_save(char_t* name);
void vm_load(char_t* name);
void vm_code_emit(byte_t* bytes, size_t len);
void vm_code_set(size_t index, byte_t* bytes, size_t len);
size_t vm_code_addr();
void vm_data_emit(byte_t* bytes, size_t len);
size_t vm_data_addr();

#endif /* VM_H */
