/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
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
