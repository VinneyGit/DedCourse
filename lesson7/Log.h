#ifndef LOG_H
#define LOG_H

//==================================================================================================

#define DEBUG_INFO_ALIAS \
__FILE__,       \
__LINE__,       \
__FUNCTION__,   \
__TIME__,       \
__DATE__

#define DEBUG_FUNCTION_ARGS \
const char*     FILE_NAME,  \
size_t          LINE,       \
const char*     FUNC,       \
const char*     TIME,       \
const char*     DATE

#define VAR_NAME(x) #x


//==================================================================================================

void    logStart    (                           );

void    log         (const char* format, ...    );

void    logClose    (                           );

//==================================================================================================

#endif
