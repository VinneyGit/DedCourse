#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include <fcntl.h>
#include <sys/stat.h>

//======================================================================================================================

// #define STACK_DEBUG
// #define MACHINE_DEBUG

#include "Comparators.c"
#include "OneginLib.c"
#include "../lesson7/Stack.h"
#include "../lesson7/Stack.c"

//======================================================================================================================

#ifdef MACHINE_DEBUG
#define ON_DBG_VM(...) __VA_ARGS__
#else
#define ON_DBG_VM(...)
#endif

//======================================================================================================================

typedef enum {
    VM_OK = 0,
    VM_ERROR = -1
} errorVM;

//======================================================================================================================

//TODO add safety for stack
//TODO add errors codes
//TODO файлик со всеми командами
//TODO забить хер на ошибки errorVM и сделать войды?

//======================================================================================================================

errorVM CommandProcessing       (int PC, double argument, Stack_t* memory
                                 ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//----------------------------------------------------------------------------------------------------------------------

void      HLT     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

void      MOV     (Stack_t* memory, double argument ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

errorVM   ADD     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
errorVM   SUB     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
errorVM   MUL     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));
errorVM   DIV     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

errorVM   OUT     (Stack_t* memory ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//======================================================================================================================

int main(int argc, char** argv) {

    //READING FILE'S NAMES
    char* INPUT_PATH = NULL;
    char* OUTPUT_PATH = NULL;

    if (GetFileNames(argc, argv, &INPUT_PATH, &OUTPUT_PATH) == ERROR) {
        return -1;
    }
    ON_DBG_VM(printf("Input: %s\n", INPUT_PATH);)
    ON_DBG_VM(printf("Output: %s\n", OUTPUT_PATH);)

    //READING AND SEPARATING COMMANDS OF RAGE BYTE CODE
    Text code = {};

    if (ReadFileSize(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    if (ReadFromFileToBuffer(INPUT_PATH, &code) == ERROR) {
        return -1;
    }

    ON_DBG_VM(printf("%s\n", code.buffer);)

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

        ON_DBG_VM(printf("PC: %d, ARG: %lg\n", PC, ARG);)

        if (PC == 0) {
            HLT(&memory);
            return 0;
        }

        errorVM err = CommandProcessing(PC, ARG, &memory ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(memory)));

        if (err == VM_ERROR) {
            printf("VM PANIC!\n");
            HLT(&memory);
            return 0;
        }
    }
}

//======================================================================================================================

errorVM CommandProcessing(int PC, double argument, Stack_t* memory
                       ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)) {

    if (PC != 1 && !DoubleComparison(argument, 1)) {
        return VM_ERROR;
    }

    switch(PC) {
        case 1:
            ON_DBG_VM(printf("MOV\n");)
            MOV(memory, argument);
            return VM_OK;
        case 2:
            ON_DBG_VM(printf("ADD\n");)
            return ADD(memory);
        case 3:
            ON_DBG_VM(printf("SUB\n");)
            return SUB(memory);
        case 4:
            ON_DBG_VM(printf("MUL\n");)
            return MUL(memory);
        case 5:
            ON_DBG_VM(printf("DIV\n");)
            return DIV(memory);
        case 6:
            ON_DBG_VM(printf("OUT\n");)
            OUT(memory);
            return VM_OK;
        default:
            ON_DBG_VM(printf("UNKNOWN COMMAND\n");)
            return VM_ERROR;
    }

    return VM_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------

void HLT(Stack_t* memory){
    StackDstr(memory ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(memory)));
}

//----------------------------------------------------------------------------------------------------------------------

void MOV(Stack_t* memory, double argument){
    StackPush(memory, argument);
}

//----------------------------------------------------------------------------------------------------------------------

errorVM ADD(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 + argument1);

    return VM_OK;
}

errorVM SUB(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 - argument1);

    return VM_OK;
}

errorVM MUL(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    StackPush(memory, argument2 * argument1);

    return VM_OK;
}

errorVM DIV(Stack_t* memory) {
    double argument1 = NAN;
    double argument2 = NAN;

    StackPop(memory, &argument1);
    StackPop(memory, &argument2);

    if (IsZero(argument1)) {
        return VM_ERROR;
    }

    StackPush(memory, argument2 / argument1);

    return VM_OK;
}

//----------------------------------------------------------------------------------------------------------------------

errorVM OUT(Stack_t* memory) {
    double argument = NAN;
    StackPop(memory, &argument);
    printf("OUT: %lg\n", argument);

    return VM_OK;
}
