#include "System.Classes.hpp"
#include <sys/time.h>

namespace System {

namespace Classes {

TCriticalSection::TCriticalSection()
{
}

void TCriticalSection::Acquire()
{
}

void TCriticalSection::Release()
{
}

// (c) http://www.doctort.org/adam/nerd-notes/linux-equivalent-of-the-windows-gettickcount-function.html
unsigned long GetTickCount()
{
    struct timeval tv;
    if(gettimeofday(&tv, nullptr) != 0) {
        return 0;
    }

    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

} // Classes


} // System
