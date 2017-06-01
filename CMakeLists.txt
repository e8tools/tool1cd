cmake_minimum_required(VERSION 2.8)

project(ctool1cd)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")

add_subdirectory(${SOURCE_DIR}/SystemClasses)
add_subdirectory(${SOURCE_DIR}/cTool1CD)
