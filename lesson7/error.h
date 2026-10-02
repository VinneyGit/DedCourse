#ifndef ERROR_H
#define ERROR_H

//==============================================================================

#include <stdio.h>

//==============================================================================



typedef struct {
    const char*     FILE_NAME;
    size_t          LINE;
    const char*     FUNC;
    const char*     TIME;
    const char*     DATE;
} ProgramEvent_t;

//==============================================================================

// void printDump(FILE* DUMP_FILE, ProgramObject_t OBJECT_INF0);

//==============================================================================

#endif
