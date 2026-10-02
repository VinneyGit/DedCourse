#ifndef ERROR_H
#define ERROR_H

//==============================================================================

#include <stdio.h>

//==============================================================================

#define PASTE_DEBUG_INFO_ALIAS \
,         \
__FILE__, \
__LINE__, \
__FUNC__, \
__TIME__, \
__DATE__

#define DEBUG_FUNCTION_ARGS \
,                     \
const char*     FILE, \
size_t          LINE, \
const char*     FUNC, \
const char*     TIME, \
const char*     DATE

#define DEBUG_INFO_TO_PROGRAM_OBJECT_STRUCT(x) \
x->DEBUG_INFO.FILE = FILE; \
x->DEBUG_INFO.LINE = LINE; \
x->DEBUG_INFO.FUNC = FUNC; \
x->DEBUG_INFO.TIME = TIME; \
x->DEBUG_INFO.DATE = DATE;

#define DEBUG_INFO_RESET(x) \
x->DEBUG_INFO.FILE = NULL; \
x->DEBUG_INFO.LINE =    0; \
x->DEBUG_INFO.FUNC = NULL; \
x->DEBUG_INFO.TIME = NULL; \
x->DEBUG_INFO.DATE = NULL;

typedef struct {
    const char*     FILE;
    size_t          LINE;
    const char*     FUNC;

    const char*     TIME;
    const char*     DATE;
} ProgramObject_t;

//==============================================================================

void printDump(FILE* DUMP_FILE, ProgramObject_t OBJECT_INF0);

//==============================================================================

#endif
