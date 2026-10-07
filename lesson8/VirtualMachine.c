#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <fcntl.h>
#include <sys/stat.h>

//----------------------------------------------------------------------------------------------------------------------

// #define STACK_DEBUG

#include "Comparators.c"
#include "../lesson7/Stack.h"
#include "../lesson7/Stack.c"

//======================================================================================================================

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    char*   str;
    size_t  len;
} String;

typedef struct {
    char*   buffer;
    size_t  bufferSize;
    size_t  charRead;
    String* lines;
    size_t  linesAmount;
    int     maxLineLen;
} Text;

typedef enum {
    OK      = 0,
    ERROR   = -1
} error;

//======================================================================================================================

error   GetFileNames            (int argc, char ** argv, char** inputPath, char** outputPath);

//----------------------------------------------------------------------------------------------------------------------

error   ReadFileSize            (const char* fileName, Text* text);
error   ReadFromFileToBuffer    (const char* fileName, Text* text);

//----------------------------------------------------------------------------------------------------------------------

void    BufferSplit             (Text* text);
error   LinesIndexing           (Text* text);

//----------------------------------------------------------------------------------------------------------------------

error   CommandProcessing       (int PC, double argument, Stack_t* memory
                                 ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//----------------------------------------------------------------------------------------------------------------------

void    HLT     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

void    PUSH    (Stack_t* memory, double argument ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

error   ADD     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
error   SUB     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
error   MUL     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
error   DIV     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

error   OUT     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//======================================================================================================================

int main(int argc, char** argv) {

    //READING FILE'S NAMES
    char* INPUT_PATH = NULL;
    char* OUTPUT_PATH = NULL;

    if (GetFileNames(argc, argv, &INPUT_PATH, &OUTPUT_PATH) == ERROR) {
        return -1;
    }
    printf("Input: %s\n", INPUT_PATH);
    printf("Output: %s\n", OUTPUT_PATH);

    //READING AND SEPARATING COMMANDS OF RAGE BYTE CODE
    Text code = {};

    if (ReadFileSize(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    if (ReadFromFileToBuffer(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    printf("%s\n", code.buffer);

    BufferSplit(&code);

    if (LinesIndexing(&code) == ERROR) {
        return -1;
    }

    //TASK MANAGER
    Stack_t memory = {};
    StackCtor(&memory ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(memory)));

    for (size_t i = 0; i < code.linesAmount; i++) {
        int PC = 0;
        double ARG = 0;

        if (sscanf(code.lines[i].str, "%d %lg", &PC, &ARG) == 1) {
            ARG = 1;
        }

        printf("PC: %d, ARG: %lg\n", PC, ARG);

        if (PC == 0) {
            HLT(&memory);
            return 0;
        }

        error err = CommandProcessing(PC, ARG, &memory ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(memory)));

        if (err == ERROR) {
            HLT(&memory);
            return 0;
        }
    }
}

//======================================================================================================================

error GetFileNames(int argc, char ** argv, char** inputPath, char** outputPath) {
    if (argc == 1) {
        printf("Please, put path for INPUT and OUTPUT files\n");
        return ERROR;
    }

    else if (argc == 2) {
        printf("Please, put path for OUTPUT file\n");
        return ERROR;
    }

    *inputPath = *(argv + 1);
    *outputPath = *(argv + 2);

    return OK;
}

//----------------------------------------------------------------------------------------------------------------------

error ReadFileSize(const char* fileName, Text* text) {
    assert(fileName);

    struct stat fileStats;
    int returnValue = stat(fileName, &fileStats);

    if (returnValue == -1) {
        printf("ERROR WHILE READING FILE'S DATA\n");
        return ERROR;
    }

    text->bufferSize = fileStats.st_size;

    return OK;
}

error ReadFromFileToBuffer(const char* fileName, Text* text) {
    assert(fileName);
    assert(text);

    text->buffer = (char*)calloc(text->bufferSize, sizeof(char));

    if (text->buffer == NULL) {
        printf("ERROR WITH MEMORY ALLOCATION FOR BUFFER\n");
        return ERROR;
    }

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        printf("ERROR WHILE OPENING INPUT FILE\n");
        return ERROR;
    }

    text->charRead = fread(text->buffer, sizeof(char), text->bufferSize, file);

    fclose(file);

    return OK;
}

//----------------------------------------------------------------------------------------------------------------------

void BufferSplit(Text* text) { //TODO EASIER
    assert(text);

    char* substr = text->buffer;

    while ((substr = strchr(substr, '\n')) != NULL) {
        *substr = '\0';
        substr++;

        text->linesAmount++;
    }
}

error LinesIndexing(Text* text) {
    assert(text);

    text->lines = (String*)calloc(text->linesAmount, sizeof(String));

    if (text->lines == NULL) {
        printf("ERROR WITH MEMORY ALLOCATION FOR LINES\n");
        return ERROR;
    }

    size_t position = 0;
    size_t stringNum = 0;

    while (position < text->bufferSize) {
        int len = 0;
        text->lines[stringNum].str = text->buffer + position;

        while (text->buffer[position] != '\0') {
            position++;
            len++;
        }
        position++;

        text->lines[stringNum].len = len;

        text->maxLineLen = max(len, text->maxLineLen);

        stringNum++;
    }

    return OK;
}

//----------------------------------------------------------------------------------------------------------------------

error CommandProcessing(int PC, double argument, Stack_t* memory
                       ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)) {

    if (PC != 1 && !DoubleComparison(argument, 1)) {
        return ERROR;
    }

    switch(PC) {
        case 1:
            printf("PUSH\n");
            PUSH(memory, argument);
            return OK;
        case 2:
            printf("ADD\n");
            return ADD(memory);
        case 3:
            printf("SUB\n");
            return SUB(memory);
        case 4:
            printf("MUL\n");
            return MUL(memory);
        case 5:
            printf("DIV\n");
            return DIV(memory);
        case 6:
            printf("OUT\n");
            OUT(memory);
            return OK;
        default:
            printf("UNKNOWN COMMAND\n");
            return ERROR;
    }

    return ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

void HLT(Stack_t* memory){
    StackDstr(memory ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(memory)));
}

//----------------------------------------------------------------------------------------------------------------------

void PUSH(Stack_t* memory, double argument){
    StackPush(memory, argument);
}

//----------------------------------------------------------------------------------------------------------------------

error ADD(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 + argument1);

    return OK;
}

error SUB(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 - argument1);

    return OK;
}

error MUL(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 * argument1);

    return OK;
}

error DIV(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    if (IsZero(argument1)) {
        return ERROR;
    }

    StackPush(memory, argument2 / argument1);

    return OK;
}

//----------------------------------------------------------------------------------------------------------------------

error OUT(Stack_t* memory) {
    double argument = NAN;
    StackPop(memory, &argument);
    printf("OUT: %lg\n", argument);

    return OK;
}
