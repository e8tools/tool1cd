cmake_minimum_required(VERSION 2.8)

project(TestProject)

file(GLOB TEST_SYSTEM_SOURCES "SystemClasses/test_*.cpp")
file(GLOB TEST_TOOL1CD_SOURCES "tool1cd/test_*.cpp")

add_executable(testproject ${TEST_SYSTEM_SOURCES} ${TEST_TOOL1CD_SOURCES} catch_main.cpp catch.hpp)

include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (testproject SystemClasses)
