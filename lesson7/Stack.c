#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#include "Stack.h"

#include "Log.h"
#include "Log.c"

//==============================================================================

//TODO add canary
//TODO print
//TODO size no errorStack
//TODO is_empty no errorStack
//TODO clear и сделать отдельную функцию перераспределения памяти?
//TODO сделать удаление повреждённого стека

//==============================================================================
/*
int main() {
    Stack_t stk1 = {};

    StackCtor(&stk1 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    StackPush(&stk1, 10 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));
    StackPush(&stk1, 20 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    double x = 0;

    StackPop(&stk1, &x ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    printf("%lg\n", x);

    for (int i = 0; i < 20; i++) {
        StackPush(&stk1, i ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));
    }

    // printf("<\n");
    for (size_t i = 20; i > 0; i--) {
        StackPop(&stk1, &x ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

        printf("<\t" PRINT_DATA_TYPE "\t>\n", x);
    }
    // printf(">\n");

    StackDstr(&stk1 ON_DBG_STACK(, DEBUG_INFO_ALIAS, VAR_NAME(stk1)));

    return 0;
}
*/
//==============================================================================

errorStack StackCtor(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

ON_DBG_STACK(logStart();)


    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    #ifndef OFF_STACK_CANARY

    size_t dataSizeWithCan = BASE_CAPACITY * sizeof(StackElem_t)
                                       + 2 * sizeof(Canary_t);

    char* rawMemory = (char*)calloc(dataSizeWithCan, 1);

    *(Canary_t*)rawMemory = CAN_DATA_START;
    *(Canary_t*)(rawMemory + sizeof(Canary_t) + dataSizeWithCan)
                                                              = CAN_DATA_FINISH;
    stack->data = (StackElem_t*)(rawMemory + 1);

    #else

    stack->data = (StackElem_t*)calloc(BASE_CAPACITY, sizeof(StackElem_t));

    #endif


    if (stack->data == NULL) {
        return MEMORY_ALLOCATION;
    }

    stack->capacity = BASE_CAPACITY;
    stack->size = 0;

    #ifdef DUMP_CTOR_DSTR
    log("[%s/%s] stack_t \"%s\" [address: %p] created by %s at %s:%zu\n",
        DATE, TIME, STACK_NAME, stack, FUNC, FILE_NAME, LINE             );
    #endif


    errorStack verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));

    if (verifyError != STACK_OK) {
        return verifyError;
    }

    return STACK_OK;
}

errorStack StackDstr(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {


    errorStack verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));

    if (verifyError != STACK_OK) {
        return verifyError;
    }


    #ifdef DUMP_BEFORE_DESTROY
    logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
    #endif


    #ifndef OFF_STACK_CANARY

    size_t dataSizeWithCan = stack->capacity * sizeof(StackElem_t)
                                       + 2 * sizeof(Canary_t);

    char* rawMemory = (char*)stack->data - sizeof(Canary_t);

    *(Canary_t*)rawMemory = CANARY_POISON;
    *(Canary_t*)(rawMemory + sizeof(Canary_t) + dataSizeWithCan)
                                                                = CANARY_POISON;

    #endif

    free(stack->data);


    stack->data = NULL;
    stack->size = POISON_VALUE;
    stack->capacity = POISON_VALUE;


    #ifdef DUMP_CTOR_DSTR
    log("[%s/%s] stack_t \"%s\" destroyed by %s at %s:%zu\n",
        DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE        );
    #endif


    ON_DBG_STACK(logClose();)

    return STACK_DESTROYED;
}

//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {


    errorStack verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));

    if (verifyError != STACK_OK) {
        return verifyError;
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
    log("[%s/%s] stack_t \"%s\" [address: %p] changed capacity by %s at %s:%zu, "
        "old/new capacity: %zu/%zu\n", DATE, TIME, STACK_NAME, stack, FUNC,
        FILE_NAME, LINE, stack->capacity, newCapacity                      );
    #endif


        stack->capacity = newCapacity;
        stack->data = newData;
    }

    stack->data[stack->size] = value;
    stack->size++;


    #ifdef DUMP_PUSH_POP
    log("[%s/%s] stack_t \"%s\" pushed by %s at %s:%zu\n", // ???Сделать вывод значения подгоняемого типа
        DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE     );
    #endif


    #ifdef DUMP_EVERY_PUSH
    logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
    #endif


    verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));


    if (verifyError != STACK_OK) {
        return verifyError;
    }


    return STACK_OK;
}

errorStack StackPop(Stack_t* stack, StackElem_t* value
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {


    errorStack verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));

    if (verifyError != STACK_OK) {
        return verifyError;
    }


    if (stack->size == 0) {
        return POP_EMPTY_STACK;
    }

    *value = stack->data[stack->size - 1];
    stack->size--;
    stack->data[stack->size] = 0;


    #ifdef DUMP_PUSH_POP
    log("[%s/%s] stack_t \"%s\" popped by %s at %s:%zu\n",
        DATE, TIME, STACK_NAME, FUNC, FILE_NAME, LINE     );
    #endif


    if (stack->capacity > stack->size * MAX_DECREASE_GAP //TODO Проверка
               && stack->capacity >= BASE_CAPACITY * MAX_DECREASE_GAP) {

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
        log("[%s/%s] stack_t \"%s\" [address: %p] changed capacity by %s at %s:%zu, "
            "old/new capacity: %zu/%zu\n", DATE, TIME, STACK_NAME, stack, FUNC,
            FILE_NAME, LINE, stack->capacity, newCapacity                      );
        #endif


        stack->capacity = newCapacity;
        stack->data = newData;
    }


    #ifdef DUMP_EVERY_POP
    logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
    #endif


    verifyError = StackVerify(stack
    ON_DBG_STACK(, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME));

    if (verifyError != STACK_OK) {
        return verifyError;
    }


    return STACK_OK;
}

//------------------------------------------------------------------------------

#ifndef OFF_VERIFY
errorStack StackVerify(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    #ifdef DUMP_VERIFY
    logFullStack(stack, FILE_NAME, LINE, FUNC, TIME, DATE, STACK_NAME);
    #endif

    if (stack->data == NULL) {
        return EMPTY_DATA_ADDRESS;
    }
// TODO проверять чистоту памяти
    if (stack->size > stack->capacity) {
        return SIZE_BIGGER_CAPACITY;
    }


    #ifdef DUMP_VERIFY
    log("[%s/%s] stack_t \"%s\" [address: %p] verified by %s at %s:%zu\n",
        DATE, TIME, STACK_NAME, stack, FUNC, FILE_NAME, LINE              );
    #endif


    return STACK_OK;
}
#else

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

errorStack StackVerify(Stack_t* stack
ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME)                  ) {

    return STACK_OK;
}

#pragma GCC diagnostic pop
#endif

//------------------------------------------------------------------------------

ON_DBG_STACK                                                                   (
errorStack logFullStack(Stack_t* stack , DEBUG_FUNCTION_ARGS,
                        const char* STACK_NAME                               ) {

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
        log("[%zu] = " PRINT_DATA_TYPE "\n", i, stack->data[i]);
    }

    log("}\n");

    return STACK_OK;
}
                                                                               )
