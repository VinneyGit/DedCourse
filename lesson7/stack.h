#ifndef STACK_H
#define STACK_H

//==============================================================================

// #define STACK_DEBUG

#define DUMP_CTOR_DSTR
#define DUMP_PUSH_POP
#define DUMP_CAPACITY_CHANGE
#define DUMP_VERIFY
// #define DUMP_EVERY_PUSH
// #define DUMP_EVERY_POP
#define DUMP_BEFORE_DESTROY

#define OFF_STACK_CANARY
// #define DELETE_CORRUPTED_STACK
// #define OFF_VERIFY

//==============================================================================

#include <stddef.h>
#include "Log.h"

//==============================================================================

#ifdef STACK_DEBUG
#define ON_DBG_STACK(...) __VA_ARGS__
#else
#define ON_DBG_STACK(...)
#endif

//==============================================================================

typedef const unsigned long long Canary_t;

typedef double StackElem_t;
#define PRINT_DATA_TYPE "%lg"

const size_t    BASE_CAPACITY           = 7;
const int       MAX_INCREASE_GAP        = 2;
const int       MAX_DECREASE_GAP        = 4;

Canary_t    CAN_STRUCT_START    = 0x7710A10; // STRUCT STA(R)T
Canary_t    CAN_STRUCT_FINISH   = 0x7F19170; // STRUCT FINIS(H)

Canary_t    CAN_DATA_START      = 0xD710A10; // DATA   STA(R)T
Canary_t    CAN_DATA_FINISH     = 0xDF19170; // DATA   FINIS(H)

Canary_t    CANARY_POISON       = 0xDEADCA11;

const unsigned long long    POISON_VALUE        = 0xDEADCE11; // DEAD CE(LL)
const unsigned long long    SWEET_VALUE         = 0xB00B1E5;

//==============================================================================

#ifndef STACK_DEBUG

#undef DUMP_CTOR_DSTR
#undef DUMP_PUSH_POP
#undef DUMP_CAPACITY_CHANGE
#undef DUMP_VERIFY
#undef DUMP_EVERY_PUSH
#undef DUMP_EVERY_POP
#undef DUMP_BEFORE_DESTROY

#endif

//==============================================================================

typedef struct {
    unsigned long long             startStruct;

    StackElem_t*    data;

    size_t          size;
    size_t          capacity;

    size_t          size_can;
    size_t          capacity_can;

    unsigned long long             finishStruct;

} Stack_t;

//==============================================================================
// TODO RENAME ERRORS
typedef enum {
    STACK_OK,
    MEMORY_ALLOCATION,
    POP_EMPTY_STACK,
    STACK_DESTROYED,

    EMPTY_STACK_ADDRESS,
    EMPTY_DATA_ADDRESS,
    SIZE_BIGGER_CAPACITY

} errorStack;

//==============================================================================


errorStack  StackCtor       (Stack_t* stack ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

errorStack  StackDstr       (Stack_t* stack ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//------------------------------------------------------------------------------

errorStack  StackPush       (Stack_t* stack, StackElem_t value
                                            ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

errorStack  StackPop        (Stack_t* stack, StackElem_t* value
                                            ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//------------------------------------------------------------------------------

errorStack  StackVerify     (Stack_t* stack ON_DBG_STACK(, DEBUG_FUNCTION_ARGS, const char* STACK_NAME));

//------------------------------------------------------------------------------

ON_DBG_STACK(
errorStack  logFullStack    (Stack_t* stack, DEBUG_FUNCTION_ARGS, const char* STACK_NAME);
)

//==============================================================================

#endif
