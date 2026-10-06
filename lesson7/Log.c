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

    fprintf(DUMP_LOG_FILE, "[%s/%s] Log started\n", __DATE__, __TIME__);
}

//------------------------------------------------------------------------------

void log(const char* format, ...) {
    if (DUMP_LOG_FILE == NULL) {
        return;
    }

    va_list argumentsList;
    va_start(argumentsList, format);

    vfprintf(DUMP_LOG_FILE, format, argumentsList);

    va_end(argumentsList);
}

//------------------------------------------------------------------------------

void logClose() {
    if (DUMP_LOG_FILE == NULL) {
        return;
    }

    fclose(DUMP_LOG_FILE);
}
