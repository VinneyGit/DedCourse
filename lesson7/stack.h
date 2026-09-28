#ifndef STACK_H
#define STACK_H

#ifdef STACK_DEBUG
#define ON_DBG_STACK(...) __VA_ARGS__
#else
#define ON_DBG_STACK(...)
#endif

typedef double StackElem_t;

const size_t    BASE_CAPACITY       = 5;
const int       MAX_INCREASE_GAP    = 2;
const int       MAX_DECREASE_GAP    = 4;

typedef struct {
    StackElem_t*    data;
    size_t          size;
    size_t          capacity;

    ON_DBG_STACK (
    const char*     FILE;
    size_t          LINE;
    const char*     FUNC;
    )

} Stack_t;

#endif
