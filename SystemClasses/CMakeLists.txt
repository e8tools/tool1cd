cmake_minimum_required (VERSION 2.8)
project (SystemClasses)

set (SYSTEM_SOURCES String.cpp System.Classes.cpp System.cpp
	System.IOUtils.cpp TFileStream.cpp TMemoryStream.cpp TStream.cpp
	TStreamReader.cpp TStreamWriter.cpp System.SysUtils.cpp)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED YES)
set (CMAKE_CXX_EXTENSIONS OFF)
add_library (SystemClasses STATIC ${SYSTEM_SOURCES})

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)

include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (SystemClasses ${Boost_LIBRARIES})
