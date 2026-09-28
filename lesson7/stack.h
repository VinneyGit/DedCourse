#ifndef STACK_H
#define STACK_H

#ifdef STACK_DEBUG
#define ON_DBG_STACK(...) __VA_ARGS__
#else
#define ON_DBG_STACK(...)
#endif

typedef double StackElem_t;

const size_t BASE_CAPACITY = 5;

typedef struct {
    StackElem_t*    data;
    size_t          size;
    size_t          capacity;

    ON_DBG_STACK (
    const char*     name;
    const char*     file;
    int             line;
    )

} Stack_t;

#endif
