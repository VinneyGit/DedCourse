#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

//==============================================================================

typedef enum {
    OK      = 0,
    ERROR   = -1
} errorStack;

//TODO add poison
//TODO ERROR TYPES

//==============================================================================

errorStack  StackCtor   (Stack_t* stack);
errorStack  StackDstr   (Stack_t* stack);

//------------------------------------------------------------------------------

errorStack  StackPush   (Stack_t* stack, StackElem_t  value);
errorStack  StackPop    (Stack_t* stack, StackElem_t* value);

//------------------------------------------------------------------------------

errorStack  StackVerify (Stack_t* stack);

//==============================================================================

int main() {
    Stack_t stk1 = {};

    StackCtor(&stk1);

    // StackPush(&stk1, 10);
    // StackPush(&stk1, 20);

    // double x = 0;

    // StackPop(&stk1, &x);

    StackDstr(&stk1);

    return 0;
}

//==============================================================================

errorStack StackCtor(Stack_t* stack) {
    if (stack == NULL) {
        return ERROR;
    }


    stack->data = (StackElem_t*)calloc(BASE_CAPACITY, sizeof(StackElem_t));

    if (stack->data == NULL) {
        return ERROR;
    }

    stack->capacity = BASE_CAPACITY;
    stack->size = 0;

    return OK;
}

errorStack StackDstr(Stack_t* stack) {
    if (stack == NULL) {
        return ERROR;
    }


    free(stack->data);

    stack->size = 0; //TODO poison value
    stack->capacity = 0;

    return OK;
}
// TODO sizeof
//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value) {
    if (stack == NULL) {
        return ERROR;
    }


    if (stack->capacity - stack->size == 0) {
        size_t newCapacity = stack->capacity * MAX_INCREASE_GAP;

        StackElem_t* newData = (StackElem_t*)realloc(stack->data, newCapacity);

        if (newData == NULL) {
            return ERROR;
        }

        stack->capacity = newCapacity;
        stack->data = newData;
    }

    stack->data[stack->size] = value;
    stack->size++;

    return OK;
}

errorStack StackPop(Stack_t* stack, StackElem_t* value) {
    if (stack == NULL) {
        return ERROR;
    }


    if (stack->size == 0) {
        return ERROR;
    }

    *value = stack->data[stack->size - 1];
    stack->size--;

    if ((double)(stack->capacity / stack->size) > MAX_DECREASE_GAP
                                && stack->capacity / 2 > 1) {
        size_t newCapacity = stack->capacity / 2;

        StackElem_t* newData = (StackElem_t*)realloc(stack->data, newCapacity);

        if (newData == NULL) {
            return ERROR;
        }

        stack->capacity = newCapacity;
        stack->data = newData;
    }

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackVerify(Stack_t* stack) {
    return OK;
}
