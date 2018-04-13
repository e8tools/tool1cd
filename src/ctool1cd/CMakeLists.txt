cmake_minimum_required (VERSION 2.8)
project (ctool1cd)

set (tool_VERSION_MAJOR 1)
set (tool_VERSION_MINOR 0)

set (CTOOL_SOURCES cTool_1CD_Main.cpp ParseCommandLine.cpp App.cpp)
set (CTOOL_HEADERS cTool_1CD_Main.h ParseCommandLine.h ErrorCode.h App.h)

add_executable (ctool1cd ${CTOOL_SOURCES} ${CTOOL_HEADERS})

include_directories (${SOURCE_DIR}/tool1cd)
include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (ctool1cd tool1cd)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (ctool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (ctool1cd ${ZLIB_LIBRARIES})

install (TARGETS ctool1cd DESTINATION bin)
