//
// Created by egolg on 20.10.2020.
//

#include "IsE_Assembler.h"
#include <stdio.h>

int readAssemblerCodeLines (char* fileName, struct String** assemblerCode, char** storage, int* nLines)
{
    assert(fileName);
    assert(assemblerCode);
    assert(storage);

    if (!fileName || !assemblerCode)
        return NULL_POINTER_ARGUMENT_ASSEMBLER;

    readToStorage (fileName, storage);
    deleteDoubleSymb (*storage, '\n');
    *nLines = countSymb (*storage, '\n');

    splitTextIntoLines (*storage, *nLines, assemblerCode);

    substituteSymb (*storage, '\n', ';');
    substituteSymb (*storage, ';', '\0');

    return NO_ASSEMBLER_ERRORS;
}

int translateAssemblerCode (String* assemblerCode, char** IsE_ByteCode, int nLines, int* nMembers, FILE* asmListing)
{
    int codeByte = 0;
    *IsE_ByteCode = (char*) calloc (24 * nLines, sizeof(char));

    char command[100] = {};

    size_t currLabelSize = 100;

    int* labels;
    labels = (int*) calloc (currLabelSize, sizeof(int));
    int* p = labels;

    for (int i = 0; i < currLabelSize ; i++)
        labels[i] = -1;

    for (int i = 0; i < nLines; i++)
    {
        int currCodeByte = codeByte;
        int commandLen = 0;

        if(sscanf ((assemblerCode + i)->chLine + commandLen, " %s %n", command, &commandLen) < 1)
            command[0] = '\0';

        #define DEF_CPU(name,value,nParams,instruction,readArgInstruction,writeDisAsmInstruction)    \
if(!strcmp(#name, command))                                                               \
{                                                                                         \
    *(*IsE_ByteCode + codeByte++) = value;                                                \
    readArgInstruction                                                                               \
    assemblerListing (currCodeByte, codeByte - currCodeByte, (*IsE_ByteCode + currCodeByte), (assemblerCode + i)->chLine, asmListing); \
    continue;\
}
        #include "data/processor_commit.IsCPU"
        #undef DEF_CPU

        int labelNum = 0;
        sscanf ((assemblerCode + i)->chLine, "%d", &labelNum);
        if (labelNum > currLabelSize)
        {
            labels =(int*) realloc (labels, sizeof(int)*2*labelNum);
            for (int j = currLabelSize; j < 2*labelNum; ++j)
                labels[j] = -1;
        }

        labels[labelNum] = codeByte;
    }

    codeByte = 0;

    for (int i = 0; i < nLines; i++) {
        int currCodeByte = codeByte;
        int commandLen = 0;

        if(sscanf ((assemblerCode + i)->chLine + commandLen, " %s %n", command, &commandLen) < 1)
            command[0] = '\0';

#define DEF_CPU(name, value, nParams, instruction, readArgInstruction, writeDisAsmInstruction)    \
if(!strcmp(#name, command))                                                               \
{                                                                                         \
    *(*IsE_ByteCode + codeByte++) = value;                                                \
    readArgInstruction                                                                               \
    assemblerListing (currCodeByte, codeByte - currCodeByte, (*IsE_ByteCode + currCodeByte), (assemblerCode + i)->chLine, asmListing); \
    continue;\
}

#include "data/processor_commit.IsCPU"

#undef DEF_CPU
        if (strchr((assemblerCode + i)->chLine, ':')) {
            int labelNum = -1;
            sscanf((assemblerCode + i)->chLine, "%d", &labelNum);

            labels[labelNum] = codeByte;
        }
        else
        {
            printf ("fatal error");
        }

    }


    *nMembers = codeByte;
    return NO_ERRORS;
}

int assemblerListing (int begN, int nMembers, char* bytes,const char* command, FILE* asmListing)
{
    assert(bytes);
    assert(command);
    assert(asmListing);

    char line[100]={};
    sprintf (line, "%04x: ", begN);
    char byte[4] = {};
    for (int i = 0; i < nMembers; ++i)
    {
        sprintf (byte, "%02x ", (unsigned char)*(bytes + i));
        strcat (line, byte);
    }
    strcat (line, command);
    strcat (line, "\n");
    int nChars = strchr (line,'\n') - line + 1;
    fwrite (line, nChars, sizeof(char), asmListing);
    return NO_ERRORS;
}

int createByteCode (char* ByteFileName, char* IsE_ByteCode,const int version,const int nMembers)
{
    char* heading = (char*) calloc (sizeof(*heading), nMembers + 12);
    strcat (heading, "KRYM");
    char* chVers = (char*) &version;

    FILE* fOutput = fopen (ByteFileName, "wb");

    for (int i = 0 ; i < 4; i++)
    {
        *(heading + 4 + i) = *(chVers + i);
    }

    chVers = (char*) &nMembers;

    for (int i = 0 ; i < 4; i++)
    {
        *(heading + 8 + i) = *(chVers + i);
    }

    fwrite (heading, 12, sizeof(char), fOutput);
    fwrite (IsE_ByteCode, nMembers, sizeof(char), fOutput);

    fclose (fOutput);

    return NO_ERRORS;
}

int findReg (char* registerName)
{
    int regN = 255;

#define DEF_REG(name,value) if (!strcmp(#name, registerName))    \
                regN = name;                                     \

#include "data/registers.IsCPU"
#undef DEF_REG
        return regN;
}

int findInput (int argc, char *argv[], char** input)
{
    if(argc > 1)
        *input = argv[1];

    else
        *input = "data/cpu.txt";

    return 0;
}






