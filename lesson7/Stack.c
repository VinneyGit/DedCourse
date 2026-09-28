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

    if ((stack->data = (StackElem_t*)calloc(BASE_CAPACITY,
                                            sizeof(StackElem_t))) == NULL) {
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
    if (stack->data != NULL) {
        return ERROR;
    }

    stack->size = 0;
    stack->capacity = 0;

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackPush(Stack_t* stack, StackElem_t value) {
    if (stack == NULL) {
        return ERROR;
    }

    if (capacity - size < 2) {
        size_t newCapacity = capacity * 2;

        StackElem_t newData = NULL;
        if ((newData = realloc(stack->data, newCapacity)) == NULL) {
            return ERROR;
        }

        stack->data = newData;
    }

    stack->data[size] = value;
    size++;

    return OK;
}

errorStack StackPop(Stack_t* stack, StackElem_t* value) {
    if (stack == NULL) {
        return ERROR;
    }

    return OK;
}

//------------------------------------------------------------------------------

errorStack StackVerify(Stack_t* stack) {
    return OK;
}
