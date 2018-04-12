cmake_minimum_required(VERSION 3.1)

project(tool1cd_project)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

option(NOGUI "disable building GUI project")

set(BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")

add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)

# Переделываем shared-runtime на static-runtime
# Это нужно, чтобы под Windows Tool1CD не требовал vcredist
if (NOGUI)

	set (Boost_USE_MULTITHREADED OFF)
	set (Boost_USE_STATIC_RUNTIME ON)

	if (MSVC)

		foreach (flag_var
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

			if (${flag_var} MATCHES "/MD")
				string (REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif (${flag_var} MATCHES "/MD")

		endforeach(flag_var)

		set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
		set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
		set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRTD")

	endif()

	if (CMAKE_COMPILER_IS_GNUCXX)
		set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
	endif()

endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED YES)
set (CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${SOURCE_DIR}/SystemClasses)
add_subdirectory(${SOURCE_DIR}/tool1cd)
add_subdirectory(${SOURCE_DIR}/ctool1cd)
add_subdirectory(${SOURCE_DIR}/tests)

if(NOT NOGUI)
	add_subdirectory(${SOURCE_DIR}/gtool1cd)
endif()

