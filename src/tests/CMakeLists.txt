cmake_minimum_required(VERSION 2.8)

project(testproject)

file(GLOB TEST_SYSTEM_SOURCES "SystemClasses/test_*.cpp")
file(GLOB TEST_TOOL1CD_SOURCES "tool1cd/test_*.cpp")

add_executable(testproject ${TEST_SYSTEM_SOURCES} ${TEST_TOOL1CD_SOURCES} catch_main.cpp catch.hpp)

add_definitions (-DCMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

include_directories (${SOURCE_DIR}/tool1cd)
include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (testproject tool1cd)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (testproject ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (testproject ${ZLIB_LIBRARIES})
