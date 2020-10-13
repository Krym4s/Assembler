//
// Created by egolg on 11.10.2020.
//
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <cctype>
#include <cstring>
#include "IsE_assembler.h"

int readAssemblerCodeLines (char* fileName, String* *assemblerCode)
{
    assert(fileName);
    assert(assemblerCode);

    if (!fileName || !assemblerCode)
        return NULL_POINTER_ARGUMENT_ASSEMBLER;

    char* storage;
    readToStorage(fileName, &storage);

    int nLines = countSymb(storage, '\n');

    splitTextIntoLines(storage, nLines, assemblerCode);

    substituteSymb(storage, ';', '\0');

    return NO_ASSEMBLER_ERRORS;
}

char* byteTranslationOfValue (double* value, int sz)
{
    char* byteAnalog = (char*) value;
    byteAnalog = (char*) realloc (byteAnalog, sz);
    char* returnStr;

    for (int i = 0; i < sz; i++)
    {
        sprintf (returnStr + 3 * i, "%x ", byteAnalog[i]);
    }

    return  returnStr;
}

int translateAssemblerCodeToIsE_ByteCode (String* assemblerCode, double** IsE_ByteCode, const int nLines, int* nCodeMembers)
{
    int nMembersOfCode;
    *IsE_ByteCode = (double*) calloc (nLines * 3,sizeof(**IsE_ByteCode));

    if (!*IsE_ByteCode)
        return NO_FREE_MEMORY_ASSEMBLER;

    for (int i = 0; i < nLines; i++)
    {
        char* tempLinePointer = (assemblerCode + i)->chLine;

        char* assemblerCommand;
        int commandLength = 0;

        sscanf ((assemblerCode + i)->chLine, " %s %n", assemblerCommand, &commandLength);

        if(!strcmp (assemblerCommand, ARGNAME(pop)))
        {
            *(*IsE_ByteCode + commandLength++) = pop;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(add)))
        {
            *(*IsE_ByteCode + commandLength++) = add;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(sub)))
        {
            *(*IsE_ByteCode + commandLength++) = sub;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(mul)))
        {
            *(*IsE_ByteCode + commandLength++) = mul;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(pow)))
        {
            *(*IsE_ByteCode + commandLength++) = pow;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(sqrt)))
        {
            *(*IsE_ByteCode + commandLength++) = sqrt;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(neg)))
        {
            *(*IsE_ByteCode + commandLength++) = neg;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(sin)))
        {
            *(*IsE_ByteCode + commandLength++) = sin;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(cos)))
        {
            *(*IsE_ByteCode + commandLength++) = cos;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(in)))
        {
            *(*IsE_ByteCode + commandLength++) = in;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(out)))
        {
            *(*IsE_ByteCode + commandLength++) = out;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(cos)))
        {
            *(*IsE_ByteCode + commandLength++) = cos;
            continue;
        }

        if(!strcmp (assemblerCommand, ARGNAME(dump)))
        {
            *(*IsE_ByteCode + commandLength++) = dump;
            continue;
        }

        if (!strcmp (assemblerCommand, ARGNAME(push)))
        {
            *(*IsE_ByteCode + commandLength++) = push;
            double currPushValue;

            if (sscanf((assemblerCode + i)->chLine + commandLength, "%lg", &currPushValue))
            {
                *(*IsE_ByteCode + commandLength++) = 0;
                *(*IsE_ByteCode + commandLength++) = currPushValue;
            }
            else
            {
                *(*IsE_ByteCode + commandLength++) = 1;

                char* registerName;
                sscanf ((assemblerCode + i)->chLine + commandLength, "%s", registerName);

                if (!strcmp (registerName, ARGNAME(rax)))
                {
                    *(*IsE_ByteCode + commandLength++) = rax;
                    continue;
                }

                if (!strcmp (registerName, ARGNAME(rbx)))
                {
                    *(*IsE_ByteCode + commandLength++) = rbx;
                    continue;
                }

                if (!strcmp (registerName, ARGNAME(rcx)))
                {
                    *(*IsE_ByteCode + commandLength++) = rcx;
                    continue;
                }

                if (!strcmp (registerName, ARGNAME(rdx)))
                {
                    *(*IsE_ByteCode + commandLength++) = rdx;
                    continue;
                }

                else
                {
                    *(*IsE_ByteCode + --commandLength) = complError;
                    return COMPILING_ERROR;
                }

            }
        }

        *(*IsE_ByteCode + commandLength++) = complError;
        return COMPILING_ERROR;

    }
}

int assemblerListing (const char* filename, String* *assemblerCode, double* IsE_ByteCode, const int nCodeMembers)
{
    assert (filename);
    assert (assemblerCode);
    assert (IsE_ByteCode);

    FILE* assemblerListing = fopen (filename, "w");

    char* str;

    int codeIdx = 0;

    while (codeIdx < nCodeMembers)
    {
        fprintf (assemblerListing, "%x%x%x%x %x%x %x%x %x")
    }


}

