//
// Created by egolg on 20.10.2020.
//

#include "IsE_Assembler.h"
#include <stdio.h>

int readAssemblerCodeLines (char* fileName, struct String* *assemblerCode, char* *storage, int* nLines)
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
    for (int i = 0; i < nLines; i++)
    {
        int currCodeByte = codeByte;
        int commandLen = 0;
        sscanf ((assemblerCode + i)->chLine + commandLen, " %s %n", command, &commandLen);

        #define DEF_CPU(name,value,nParams,instruction)                                   \
if(!strcmp(#name, command))                                                               \
{                                                                                         \
    *(*IsE_ByteCode + codeByte++) = value;                                                \
                                                                                          \
    if (nParams)                                                                          \
    {                                                                                     \
         double dValue = 0;                                                               \
                                                                                          \
         if (sscanf ((assemblerCode + i)->chLine + commandLen, " %lg", &dValue) == 1)     \
         {                                                                                \
            *(*IsE_ByteCode + codeByte++) = 0;                                            \
            char* charDTranslit =(char*) &dValue;                                         \
            for (int j = 0; j < sizeof (double); ++j)                                     \
                *(*IsE_ByteCode + codeByte++) = *(charDTranslit + j);                     \
         }                                                                                \
         else if (!strcmp (#name, "POP"))                                                 \
            *(*IsE_ByteCode + codeByte++) = 0;                                            \
         else if (sscanf ((assemblerCode + i)->chLine + commandLen, " %s", command) != -1)\
         {                                                                                \
            *(*IsE_ByteCode + codeByte++) = 1;                                            \
            if (!strcmp("rax", command))                                                  \
                *(*IsE_ByteCode + codeByte++) = rax;                                      \
            else if (!strcmp("rbx", command))                                             \
                *(*IsE_ByteCode + codeByte++) = rbx;                                      \
            else if (!strcmp("rcx", command))                                             \
                *(*IsE_ByteCode + codeByte++) = rcx;                                      \
            else if (!strcmp("rdx", command))                                             \
                *(*IsE_ByteCode + codeByte++) = rdx;                                      \
            else *(*IsE_ByteCode + codeByte++) = 255;                                     \
         }                                                                                \
    }                                                                                     \
}

        #include "data/processor_commit.IsCPU"
        #undef DEF_CPU

        assemblerListing (currCodeByte, codeByte - currCodeByte, (*IsE_ByteCode + currCodeByte), (assemblerCode + i)->chLine, asmListing);
    }
    *nMembers = codeByte - 1;
    return NO_ERRORS;
}

int assemblerListing (int begN, int nMembers, char* bytes,const char* command, FILE* asmListing)
{
    assert(bytes);
    assert(command);
    assert(asmListing);

    char line[100]={};
    sprintf (line, "%04x: ", (char)begN);
    char byte[4] = {};
    for (int i = 0; i < nMembers; ++i)
    {
        sprintf (byte, "%02x ", (unsigned char)*(bytes + i));
        strcat (line, byte);
    }
    //sprintf(line,"%30s ", line);
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

    printf ("%s", heading);

    fwrite (heading, 12, sizeof(char), fOutput);
    fwrite (IsE_ByteCode, nMembers, sizeof(char), fOutput);

    fclose (fOutput);

    return NO_ERRORS;

}



