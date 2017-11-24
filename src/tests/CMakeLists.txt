cmake_minimum_required(VERSION 2.8)

project(TestProject)

file(GLOB TEST_SYSTEM_SOURCES "SystemClasses/test_*.cpp")
file(GLOB TEST_TOOL1CD_SOURCES "tool1cd/test_*.cpp")

if (CMAKE_VERSION VERSION_LESS "3.1")
	if(CMAKE_COMPILER_IS_GNUCXX)
		set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
	endif()
else()
	set (CMAKE_CXX_STANDARD 11)
endif()

add_executable(testproject ${TEST_SYSTEM_SOURCES} ${TEST_TOOL1CD_SOURCES} catch_main.cpp catch.hpp)

include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (testproject SystemClasses)
