#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#define STACK_DEBUG

#define DUMP_CTOR_DSTR
#define DUMP_PUSH_POP
#define DUMP_CAPACITY_CHANGE
#define DUMP_VERIFY
#define DUMP_EVERY_PUSH
#define DUMP_EVERY_POP
#define DUMP_BEFORE_DESTROY
#define DUMP_EVERY_VERIFY

#define OFF_CANARY

//------------------------------------------------------------------------------

#ifndef STACK_DEBUG

#undef DUMP_CTOR_DSTR
#undef DUMP_PUSH_POP
#undef DUMP_CAPACITY_CHANGE
#undef DUMP_VERIFY
#undef DUMP_EVERY_PUSH
#undef DUMP_EVERY_POP
#undef DUMP_BEFORE_DESTROY
#undef DUMP_EVERY_VERIFY

#endif

//------------------------------------------------------------------------------

#include "Error.h"
#include "Stack.h"
#include "Log.c"

//==============================================================================

typedef enum {
    OK                      = 0,
    EMPTY_STACK_ADDRESS     = 1,
    MEMORY_ALLOCATION       = 2,
    POP_EMPTY_STACK         = 3,
    STACK_DESTROYED         = 4
} errorStack;

//TODO add canary
//TODO print
//TODO size no errorStack
//TODO is_empty no errorStack
//TODO clear

//==============================================================================

errorStack  StackCtor       (Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

errorStack  StackDstr       (Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

//------------------------------------------------------------------------------

errorStack  StackPush       (Stack_t* stack, StackElem_t  value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

errorStack  StackPop        (Stack_t* stack, StackElem_t* value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

//------------------------------------------------------------------------------

errorStack  StackVerify     (Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

//------------------------------------------------------------------------------

errorStack  StackPrint      (Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

errorStack  logFullStack    (Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                   );

//==============================================================================

int main() {
    Stack_t stk1 = {};

    StackCtor(&stk1 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    StackPush(&stk1, 10 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));
    StackPush(&stk1, 20 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    double x = 0;

    StackPop(&stk1, &x ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    printf("%lg\n", x);

    for (int i = 0; i < 67; i++) {
        StackPush(&stk1, i ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));
    }

    StackDstr(&stk1 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    return 0;
}

//==============================================================================

errorStack StackCtor(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

ON_DBG_STACK(logStart();)

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    stack->data = (StackElem_t*)calloc(BASE_CAPACITY, sizeof(StackElem_t));

    if (stack->data == NULL) {
        return MEMORY_ALLOCATION;
    }

    stack->capacity = BASE_CAPACITY;
    stack->size = 0;


#ifdef DUMP_CTOR_DSTR
log("[%s/%s] stack_t \"%s\" [address: %p] created by %s at %s:%zu\n",
    DATE, TIME, STACK_NAME, stack, FUNC, FILE_NAME, LINE             );
#endif


    return OK;
}

errorStack StackDstr(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {


#ifdef DUMP_BEFORE_DESTROY
logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
#endif


    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    free(stack->data);

    stack->data = NULL;
    stack->size = POISON_VALUE;
    stack->capacity = POISON_VALUE;


#ifdef DUMP_CTOR_DSTR
log("[%s/%s] stack_t \"%s\" destroyed by %s at %s:%zu\n",
    DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE        );
#endif


    return STACK_DESTROYED;
}

//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    if (stack->capacity - stack->size == 0) {
        size_t newCapacity = stack->capacity * MAX_INCREASE_GAP;

        StackElem_t* newData = (StackElem_t*)realloc(stack->data,
                                             newCapacity * sizeof(StackElem_t));

        if (newData == NULL) {
            return MEMORY_ALLOCATION;
        }

        for (size_t i = stack->size; i < newCapacity; i++) {
            newData[i] = 0;
        }


#ifdef DUMP_CAPACITY_CHANGE
log("[%s/%s] stack_t \"%s\" [address: %p] changed capacity by %s at %s:%zu"
    "old/new capacity: %zu/%zu\n", DATE, TIME, STACK_NAME, stack, FUNC,
    FILE_NAME, LINE, stack->capacity, newCapacity                      );
#endif


        stack->capacity = newCapacity;
        stack->data = newData;
    }

    stack->data[stack->size] = value;
    stack->size++;


#ifdef DUMP_PUSH_POP
log("[%s/%s] stack_t \"%s\" pushed by %s at %s:%zu\n", // Сделать вывод значения подгоняемого типа
    DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE     );
#endif

#ifdef DUMP_EVERY_PUSH
logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
#endif


    return OK;
}

errorStack StackPop(Stack_t* stack, StackElem_t* value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    if (stack->size == 0) {
        return POP_EMPTY_STACK;
    }

    *value = stack->data[stack->size - 1];
    stack->size--;


#ifdef DUMP_PUSH_POP
log("[%s/%s] stack_t \"%s\" popped by %s at %s:%zu\n",
    DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE     );
#endif


    if ((double)(stack->capacity / stack->size) > MAX_DECREASE_GAP //TODO Проверка
               && (double)stack->capacity / MAX_DECREASE_GAP >= BASE_CAPACITY) {

        size_t newCapacity = stack->capacity / MAX_DECREASE_GAP;

        StackElem_t* newData = (StackElem_t*)realloc(stack->data,
                                             newCapacity * sizeof(StackElem_t));

        if (newData == NULL) {
            return MEMORY_ALLOCATION;
        }

        for (size_t i = stack->size; i < newCapacity; i++) {
            newData[i] = 0;
        }


#ifdef DUMP_CAPACITY_CHANGE
log("[%s/%s] stack_t \"%s\" [address: %p] changed capacity by %s at %s:%zu"
    "old/new capacity: %zu/%zu\n", DATE, TIME, STACK_NAME, stack, FUNC,
    FILE_NAME, LINE, stack->capacity, newCapacity                      );
#endif


        stack->capacity = newCapacity;
        stack->data = newData;
    }


#ifdef DUMP_EVERY_POP
logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
#endif


    return OK;
}

//------------------------------------------------------------------------------
/*
errorStack StackVerify(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    return OK;
}
*/
//------------------------------------------------------------------------------

errorStack logFullStack(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    log("[%s/%s] stack_t \"%s\" [address: %p] printed by %s at %s:%zu\n",
    DATE, TIME, STACK_NAME, stack, FUNC, FILE_NAME, LINE                 );

    log("||capacity = %zu\n||size = %zu\n||data[%p]\n{\n",
        stack->capacity, stack->size, stack->data    );

    for (size_t i = 0; i < stack->capacity; i++) {
        if (i < stack->size) {
            log("   *");
        }
        else {
            log("    ");
        }
        log("[%zu] = " PRINT_DATA "\n", i, stack->data[i]);
    }

    log("}\n");

    return OK;
}
