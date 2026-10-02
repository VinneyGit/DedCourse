#ifndef STACK_H
#define STACK_H

//==============================================================================

#ifdef STACK_DEBUG
#define ON_DBG_STACK(...) __VA_ARGS__
#else
#define ON_DBG_STACK(...)
#endif

//==============================================================================

#include "Error.h"

//==============================================================================

typedef double StackElem_t;

//TODO сделать отдельные cap и size для canary

const size_t    BASE_CAPACITY           = 7;
const int       MAX_INCREASE_GAP        = 2;
const int       MAX_DECREASE_GAP        = 4;

const ull       CAN_STRUCT_START        = 0xCA97710A10; // CA(N) STRUCT STA(R)T
const ull       CAN_STRUCT_FINISH       = 0xCA97F1917;  // CA(N) STRUCT FINIS(H)

const ull       CAN_DATA_START          = 0xCA9D710A10; // CA(N) DATA   STA(R)T
const ull       CAN_DATA_FINISH         = 0xCA9DF1917;  // CA(N) DATA   FINIS(H)

const ull       POISON_VALUE            = 0xDEADCE117;  // DEAD CE(LLS)

//==============================================================================

typedef struct {
    ull             startStruct;

    StackElem_t*    data;

    size_t          size;
    size_t          capacity;

    size_t          size_can;
    size_t          capacity_can;

    ull             finishStruct;

ON_DBG_STACK (
ProgramObject_t     DEBUG_INFO;
)

} Stack_t;

//==============================================================================

#endif
