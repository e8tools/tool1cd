cmake_minimum_required(VERSION 3.0)
project(gtool1cd)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(HEADER_FILES mainwindow.h starter.h littlelogwindow.h)
set(UI_FILES mainwindow.ui starter.ui littlelogwindow.ui)
set(SOURCE_FILES main.cpp mainwindow.cpp starter.cpp littlelogwindow.cpp)

add_executable(gtool1cd ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES})

include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (gtool1cd SystemClasses)

include_directories (${SOURCE_DIR}/tool1cd)
target_link_libraries (gtool1cd tool1cd)

add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)
set (Boost_USE_STATIC_RUNTIME ON)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${ZLIB_LIBRARIES})

install (TARGETS gtool1cd DESTINATION bin)

qt5_use_modules(gtool1cd Widgets)
