#ifndef K_OS_PROCESS_H
#define K_OS_PROCESS_H

#include "ProcessException.h"

#if defined(__linux__)
    #include "ProcessLinux.h"
#elif defined(_WINDOWS)
    #include "ProcessWindows.h"
#endif

#endif // K_OS_PROCESS_H
