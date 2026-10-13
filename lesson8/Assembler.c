#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <fcntl.h>
#include <sys/stat.h>

//======================================================================================================================

#define ASSEMBLER_DEBUG

#include "OneginLib.c"

//======================================================================================================================

#ifdef ASSEMBLER_DEBUG
#define ON_DBG_ASM(...) __VA_ARGS__
#else
#define ON_DBG_ASM(...)
#endif

//======================================================================================================================

void    CommandRecognize        (const char* PC, double argument);

//----------------------------------------------------------------------------------------------------------------------

void    ExecutableOpen          (const char* outputPath );

void    CommandPrintf           (const char* format, ...);

void    ExecutableClose         (                       );

//======================================================================================================================

FILE* EXECUTABLE_FILE = NULL;

//======================================================================================================================

int main(int argc, char** argv) {

    //READING FILE'S NAMES
    char* INPUT_PATH = NULL;
    char* OUTPUT_PATH = NULL;

    if (GetFileNames(argc, argv, &INPUT_PATH, &OUTPUT_PATH) == ERROR) {
        return -1;
    }
    ON_DBG_ASM(printf("Input: %s\n", INPUT_PATH);)
    ON_DBG_ASM(printf("Output: %s\n", OUTPUT_PATH);)

    //READING AND SEPARATING COMMANDS OF RAGE BYTE CODE
    Text code = {};

    if (ReadFileSize(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    if (ReadFromFileToBuffer(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    ON_DBG_ASM(printf("%s\n", code.buffer);)

    BufferSplit(&code);

    if (LinesIndexing(&code) == ERROR) {
        return -1;
    }

    ExecutableOpen(OUTPUT_PATH);

    for (size_t i = 0; i < code.linesAmount; i++) {
        char PC[8] = {0};
        double ARG = 0;

        if (sscanf(code.lines[i].str, "%s %lg", PC, &ARG) == 1) {
            ARG = 1;
        }

        ON_DBG_ASM(printf("PC: %s, ARG: %lg\n", PC, ARG);)

        CommandRecognize(PC, ARG);
    }

    ExecutableClose();
}

//======================================================================================================================

void CommandRecognize(const char* PC, double argument) {

    if (strcmp(PC, "HLT") == 0) {
        CommandPrintf("%d\n", "0");
    }

    else if (strcmp(PC, "MOV") == 0) {
        CommandPrintf("%d %lg\n", "1", argument);
    }

    else if (strcmp(PC, "ADD") == 0) {
        CommandPrintf("%d\n", "2");
    }

    else if (strcmp(PC, "SUB") == 0) {
        CommandPrintf("%d\n", "3");
    }

    else if (strcmp(PC, "MUL") == 0) {
        CommandPrintf("%d\n", "4");
    }

    else if (strcmp(PC, "DIV") == 0) {
        CommandPrintf("%d\n", "5");
    }

    else if (strcmp(PC, "DIV") == 0) {
        CommandPrintf("%d\n", "6");
    }

    else {
        ON_DBG_ASM(printf("UNKNOWN COMMAND\n");)
    }
}

//----------------------------------------------------------------------------------------------------------------------

void ExecutableOpen(const char* outputPath) {

    if (EXECUTABLE_FILE != NULL) {
        return;
    }

    EXECUTABLE_FILE = fopen(outputPath, "wb");

    assert(EXECUTABLE_FILE);
}

void CommandPrintf(const char* format, ...) {

    if (EXECUTABLE_FILE == NULL) {
        return;
    }

    va_list argumentsList;
    va_start(argumentsList, format);

    vfprintf(EXECUTABLE_FILE, format, argumentsList);

    va_end(argumentsList);
}

void ExecutableClose() {

    if (EXECUTABLE_FILE == NULL) {
        return;
    }

    fclose(EXECUTABLE_FILE);
}

