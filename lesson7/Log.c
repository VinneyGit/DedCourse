#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

//==============================================================================

#define DEBUG_INFO_ALIAS \
__FILE__,       \
__LINE__,       \
__FUNCTION__,   \
__TIME__,       \
__DATE__

#define DEBUG_FUNCTION_ARGS \
const char*     FILE_NAME,  \
size_t          LINE,       \
const char*     FUNC,       \
const char*     TIME,       \
const char*     DATE

#define VAR_NAME(x) #x

//==============================================================================

FILE* DUMP_LOG_FILE = NULL;
// Добавить время?
// Имя файла?
//==============================================================================

void    logStart    (                                                         );

void    log         (const char* format, ...                                  );

void    logClose    (                                                         );

//==============================================================================

void logStart() {
    if (DUMP_LOG_FILE != NULL) {
        return;
    }

    DUMP_LOG_FILE = fopen("dump.log", "w");

    assert(DUMP_LOG_FILE);

    fprintf(DUMP_LOG_FILE, "Log started\n");
}

//------------------------------------------------------------------------------

void log(const char* format, ...) {
    va_list argumentsList;
    va_start(argumentsList, format);

    if (DUMP_LOG_FILE == NULL) {
        return;
    }

    vfprintf(DUMP_LOG_FILE, format, argumentsList);

    va_end(argumentsList);
}

//------------------------------------------------------------------------------

void logClose() {
    fclose(DUMP_LOG_FILE);
}
