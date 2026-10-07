#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

//==============================================================================

FILE* DUMP_LOG_FILE = NULL;
// Добавить время?
// Имя файла?
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
