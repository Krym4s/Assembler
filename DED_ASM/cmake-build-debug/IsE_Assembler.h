//
// Created by egolg on 20.10.2020.
//

#ifndef DED_ASM_ISE_ASSEMBLER_H
#define DED_ASM_ISE_ASSEMBLER_H

#include "IsE_Stack.h"
#include "IsE_text.h"

enum Registers
{
#define DEF_REG(name,value) name = value,
#include "data/registers.IsCPU"
#undef DEF_REG
};

enum AssemblerCodeErrors
{
    NO_ASSEMBLER_ERRORS             = 0,
    NULL_POINTER_ARGUMENT_ASSEMBLER = -1,
    NO_FREE_MEMORY_ASSEMBLER        = -2,
    COMPILING_ERROR                 = -3,
};

int readAssemblerCodeLines (char* fileName, struct String** assemblerCode, char** storage, int* nLines);

int translateAssemblerCode (String* assemblerCode, char** IsE_ByteCode, int nLines, int* nMembers, FILE* asmListing);

int assemblerListing (int begN, int nMembers, char* bytes,const char* command, FILE* asmListing);

int createByteCode (char* ByteFileName, char* IsE_ByteCode, const int version, const int nMembers);

int findReg (char* registerName);

int findInput (int argc, char *argv[], char** input);

#endif //DED_ASM_ISE_ASSEMBLER_H
