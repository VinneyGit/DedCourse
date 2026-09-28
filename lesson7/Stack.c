#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#define STACK_DEBUG
#define CAPACITY_CHANGE_PRINT

//------------------------------------------------------------------------------

#include "error.h"
#include "stack.h"

//==============================================================================

typedef enum {
    OK                      = 0,
    EMPTY_STACK_ADDRESS     = 1,
    MEMORY_ALLOCATION       = 2,
    POP_EMPTY_STACK         = 3
} errorStack;

//TODO add poison
//==============================================================================

errorStack  StackCtor   (Stack_t* stack
ON_DBG_STACK(, const char* FILENAME, const size_t LINE, const char* FUNC));

errorStack  StackDstr   (Stack_t* stack);

//------------------------------------------------------------------------------

errorStack  StackPush   (Stack_t* stack, StackElem_t  value);
errorStack  StackPop    (Stack_t* stack, StackElem_t* value);

//------------------------------------------------------------------------------

errorStack  StackVerify (Stack_t* stack);

//==============================================================================

int main() {
    Stack_t stk1 = {};

    StackCtor(&stk1 ON_DBG_STACK(, __FILE__, __LINE__, __FUNCTION__));

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
ON_DBG_STACK(, const char* FILENAME, const size_t LINE, const char* FUNC)) {

ON_DBG_STACK(
stack->FILE = FILENAME;
stack->LINE = LINE;
stack->FUNC = FUNC;
)

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    stack->data = (StackElem_t*)calloc(BASE_CAPACITY, sizeof(StackElem_t));

    if (stack->data == NULL) {
        return MEMORY_ALLOCATION;
    }

    stack->capacity = BASE_CAPACITY;
    stack->size = 0;

    return OK;
}

errorStack StackDstr(Stack_t* stack) {

ON_DBG_STACK(
stack->FILE = NULL;
stack->LINE = 0;
stack->FUNC = NULL;
)

    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }


    free(stack->data);

    stack->size = 0; // TODO poison value
    stack->capacity = 0;

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value) {
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

errorStack StackPop(Stack_t* stack, StackElem_t* value) {
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

errorStack StackVerify(Stack_t* stack) {
    if (stack == NULL) {
        return EMPTY_STACK_ADDRESS;
    }

    return OK;
}
