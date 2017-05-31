cmake_minimum_required (VERSION 2.8)
project (SystemClasses)

set (SYSTEM_SOURCES String.cpp System.Classes.cpp System.cpp
	System.IOUtils.cpp TFileStream.cpp TMemoryStream.cpp TStream.cpp
	TStreamReader.cpp TStreamWriter.cpp System.SysUtils.cpp)
set (SYSTEM_HEADERS String.hpp System.Classes.hpp System.hpp
	System.IOUtils.hpp TFileStream.hpp TMemoryStream.hpp TStream.hpp
	TStreamReader.hpp TStreamWriter.hpp System.SysUtils.hpp)

if (CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
	endif()
else()
	set (CMAKE_CXX_STANDARD 11)
	set (CMAKE_CXX_STANDARD_REQUIRED YES)
	set (CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library (SystemClasses STATIC ${SYSTEM_SOURCES} ${SYSTEM_HEADERS})

add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)
set (Boost_USE_STATIC_RUNTIME ON)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)

include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (SystemClasses ${Boost_LIBRARIES})
