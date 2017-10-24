#include "GetTickCount.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif // _WIN32

namespace System {

namespace Classes {

// (c) http://www.doctort.org/adam/nerd-notes/linux-equivalent-of-the-windows-gettickcount-function.html
unsigned long GetTickCount()
{
    #ifdef _WIN32
    return (unsigned long)(::GetTickCount());
    #else
    struct timeval tv;
    if(gettimeofday(&tv, nullptr) != 0) {
        return 0;
    }

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    #endif // _WIN32
}

} // Classe

} // System
