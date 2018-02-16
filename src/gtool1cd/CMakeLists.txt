cmake_minimum_required(VERSION 3.0)
project(gtool1cd)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(HEADER_FILES mainwindow.h starter.h littlelogwindow.h cache.h table_fields_window.h table_data_window.h
	skobkatextwindow.h models/tables_list_model.h models/fields_list_model.h models/table_data_model.h)
set(UI_FILES mainwindow.ui starter.ui littlelogwindow.ui table_fields_window.ui table_data_window.ui
	skobkatextwindow.ui)
set(SOURCE_FILES main.cpp mainwindow.cpp starter.cpp littlelogwindow.cpp cache.cpp table_fields_window.cpp table_data_window.cpp
	skobkatextwindow.cpp)

add_executable(gtool1cd WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES})

include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (gtool1cd SystemClasses)

include_directories (${SOURCE_DIR}/tool1cd)
target_link_libraries (gtool1cd tool1cd)


find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${ZLIB_LIBRARIES})

install (TARGETS gtool1cd DESTINATION bin)

qt5_use_modules(gtool1cd Widgets)
