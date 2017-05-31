cmake_minimum_required (VERSION 2.8)
project (tool1cd)

set (tool_VERSION_MAJOR 1)
set (tool_VERSION_MINOR 0)

# Переделываем shared-runtime на static-runtime
# Это нужно, чтобы под Windows Tool1CD не требовал vcredist
if (MSVC)

	foreach (flag_var 
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

		if (${flag_var} MATCHES "/MD")
			string (REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif (${flag_var} MATCHES "/MD")

	endforeach(flag_var)

	set (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRT")

endif()

set (TOOL_SOURCES cTool_1CD_Main.cpp ParseCommandLine.cpp MessageRegistration.cpp Class_1CD.cpp
	Common.cpp ConfigStorage.cpp Parse_tree.cpp APIcfBase.cpp TempStream.cpp Base64.cpp UZLib.cpp Messenger.cpp)
set (TOOL_HEADERS cTool_1CD_Main.h ParseCommandLine.h MessageRegistration.h Class_1CD.h
	Common.h ConfigStorage.h Parse_tree.h APIcfBase.h TempStream.h Base64.h UZLib.h Messenger.h ErrorCode.h)

if (CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
	endif()
else()
	set (CMAKE_CXX_STANDARD 11)
endif()

add_executable (ctool1cd ${TOOL_SOURCES} ${TOOL_HEADERS})

add_subdirectory (SystemClasses)
include_directories (SystemClasses)
target_link_libraries (ctool1cd SystemClasses)

add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)
set (Boost_USE_STATIC_RUNTIME ON)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (ctool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (ctool1cd ${ZLIB_LIBRARIES})

install (TARGETS ctool1cd DESTINATION bin)
