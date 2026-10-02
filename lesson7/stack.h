#ifndef STACK_H
#define STACK_H

//==============================================================================

#ifdef STACK_DEBUG
#define ON_DBG_STACK(...) __VA_ARGS__
#else
#define ON_DBG_STACK(...)
#endif

//==============================================================================



//==============================================================================

typedef double StackElem_t;
#define PRINT_DATA "%lg"

//TODO сделать отдельные cap и size для canary

const size_t    BASE_CAPACITY           = 7;
const int       MAX_INCREASE_GAP        = 2;
const int       MAX_DECREASE_GAP        = 4;

const unsigned long long    CAN_STRUCT_START    = 0x7710A10; // STRUCT STA(R)T
const unsigned long long    CAN_STRUCT_FINISH   = 0x7F19170; // STRUCT FINIS(H)

const StackElem_t           CAN_DATA_START      = 0xD710A10; // DATA   STA(R)T
const StackElem_t           CAN_DATA_FINISH     = 0xDF19170; // DATA   FINIS(H)

const unsigned long long    POISON_VALUE        = 0xDEADCE11; // DEAD CE(LL)
const unsigned long long    SWEET_VALUE         = 0xB00B1E55;

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

#endif
