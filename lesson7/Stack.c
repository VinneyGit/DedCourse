#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#define STACK_DEBUG
#define CAPACITY_CHANGE_PRINT

//------------------------------------------------------------------------------

#include "Error.h"
#include "Stack.h"

//==============================================================================

typedef enum {
    OK                      = 0,
    EMPTY_STACK_ADDRESS     = 1,
    MEMORY_ALLOCATION       = 2,
    POP_EMPTY_STACK         = 3
} errorStack;

//TODO add canary
//TODO print
//TODO size
//TODO is_empty
//TODO clear
// Идея сделать общий тип структур program obj в котором будет храниться DEBUG
// INFO, а при печати в дамп передавать функцию печати объекта, а в самом стеке
// хранится сам program obj

//==============================================================================

errorStack  StackCtor   (Stack_t* stack
ON_DBG_STACK(DEBUG_FUNCTION_ARGS, FILE* DUMP_FILE)                            );

errorStack  StackDstr   (Stack_t* stack
ON_DBG_STACK(, FILE* DUMP_FILE)                                               );

//------------------------------------------------------------------------------

errorStack  StackPush   (Stack_t* stack, StackElem_t  value
ON_DBG_STACK(, FILE* DUMP_FILE)                                               );

errorStack  StackPop    (Stack_t* stack, StackElem_t* value
ON_DBG_STACK(, FILE* DUMP_FILE)                                               );

//------------------------------------------------------------------------------

errorStack  StackVerify (Stack_t* stack
ON_DBG_STACK(, FILE* DUMP_FILE)                                               );

//------------------------------------------------------------------------------

errorStack  StackPrint  (Stack_t* stack
ON_DBG_STACK(, FILE* DUMP_FILE)                                               );

//==============================================================================

int main() {
ON_DBG_STACK(
FILE* DUMP_FILE = fopen("dump.log", "w");
)

    Stack_t stk1 = {};

    StackCtor(&stk1
ON_DBG_STACK(PASTE_DEBUG_INFO_ALIAS, DUMP_FILE)
    );

    StackPush(&stk1, 10);
    StackPush(&stk1, 20);

    double x = 0;

    StackPop(&stk1, &x);

ON_DBG_STACK(
printf("STACK FILE: %s\nSTACK LINE: %zu\nSTACK_FUNC: %s\n",
        stk1.FILE, stk1.LINE, stk1.FUNC);
)

    printf("%lg\n", x);

    StackDstr(&stk1);

    return 0;
}

//==============================================================================

errorStack StackCtor(Stack_t* stack
ON_DBG_STACK(DEBUG_FUNCTION_ARGS, FILE* DUMP_FILE)                           ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

ON_DBG_STACK(DEBUG_INFO_TO_PROGRAM_OBJECT_STRUCT(stack))

    stack->data = (StackElem_t*)calloc(BASE_CAPACITY, sizeof(StackElem_t));

    if (stack->data == NULL) {
        return MEMORY_ALLOCATION;
    }

    stack->capacity = BASE_CAPACITY - 2;
    stack->size = 0;

    return OK;
}

errorStack StackDstr(Stack_t* stack
ON_DBG_STACK(, FILE* DUMP_FILE)                                              ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

ON_DBG_STACK(DEBUG_INFO_RESET(stack))

    free(stack->data);

    stack->size = POISON_VALUE0;
    stack->capacity = POISON_VALUE;

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value
ON_DBG_STACK(, FILE* DUMP_FILE)                                              ) {

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

#ifdef CAPACITY_CHANGE_PRINT
printf("CAPACITY INCREASED, OLD/NEW CAPACITY: %zu/%zu\n",
                                stack->capacity, newCapacity);
#endif

        stack->capacity = newCapacity;
        stack->data = newData;
    }

    stack->data[stack->size] = value;
    stack->size++;

    return OK;
}

errorStack StackPop(Stack_t* stack, StackElem_t* value
ON_DBG_STACK(, FILE* DUMP_FILE)                                              ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    if (stack->size == 0) {
        return POP_EMPTY_STACK;
    }

    *value = stack->data[stack->size - 1];
    stack->size--;

    if ((double)(stack->capacity / stack->size) > MAX_DECREASE_GAP
               && (double)stack->capacity / MAX_DECREASE_GAP >= BASE_CAPACITY) {

        size_t newCapacity = stack->capacity / MAX_DECREASE_GAP;

        StackElem_t* newData = (StackElem_t*)realloc(stack->data,
                                             newCapacity * sizeof(StackElem_t));

        if (newData == NULL) {
            return MEMORY_ALLOCATION;
        }

#ifdef CAPACITY_CHANGE_PRINT
printf("CAPACITY DECREASED, OLD/NEW CAPACITY: %zu/%zu\n",
                                stack->capacity, newCapacity);
#endif

        stack->capacity = newCapacity;
        stack->data = newData;
    }

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackVerify(Stack_t* stack
ON_DBG_STACK(, FILE* DUMP_FILE)                                              ) {

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    return OK;
}
