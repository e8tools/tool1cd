cmake_minimum_required (VERSION 2.8)
project (SystemClasses)

set (SYSTEM_SOURCES String.cpp System.Classes.cpp System.cpp
	System.IOUtils.cpp TFileStream.cpp TMemoryStream.cpp TStream.cpp
	TStreamReader.cpp TStreamWriter.cpp System.SysUtils.cpp
	GetTickCount.cpp)
set (SYSTEM_HEADERS String.hpp System.Classes.hpp System.hpp
	System.IOUtils.hpp TFileStream.hpp TMemoryStream.hpp TStream.hpp
	TStreamReader.hpp TStreamWriter.hpp System.SysUtils.hpp
	GetTickCount.hpp)

add_library (SystemClasses STATIC ${SYSTEM_SOURCES} ${SYSTEM_HEADERS})

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)

include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (SystemClasses ${Boost_LIBRARIES})
