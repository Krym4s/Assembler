//
// Created by egolg on 11.10.2020.
//

#ifndef PROCESSOR_ISE_ASSEMBLER_H
#define PROCESSOR_ISE_ASSEMBLER_H

#include "C:\MY_CPP_LIBS\IsE_text.h"

#define ARGNAME(x) #x

enum ProcessorRegisters
{
    rax = 0,
    rbx = 1,
    rcx = 2,
    rdx = 3
};

enum CommandNumbers
{
    add  = 1,
    sub  = 129,
    mul  = 2,
    div  = 130,
    in   = 3,
    out  = 131,
    sin  = 115,
    cos  = 99,
    pow  = 112,
    sqrt = 240,
    dump = 100,
    push = 4,
    pop  = 132,
    neg  = 126,
    complError = 255,
    hlt  = 0
};

enum AssemblerCodeErrors
{
    NO_ASSEMBLER_ERRORS             = 0,
    NULL_POINTER_ARGUMENT_ASSEMBLER = -1,
    NO_FREE_MEMORY_ASSEMBLER        = -2,
    COMPILING_ERROR                 = -3,
};

int readAssemblerCodeLines (char* fileName, String* *assemblerCode);

int translateAssemblerCodeToIsE_ByteCode (String* assemblerCode, char* IsE_ByteCode);

char* byteTranslationOfValue (double* value, int sz);

int assemblerListing (char* filename, String* *assemblerCode, double* IsE_ByteCode);

#endif //PROCESSOR_ISE_ASSEMBLER_H
