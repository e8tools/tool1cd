cmake_minimum_required(VERSION 3.0)
project(tool1cd)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set(SOURCE_FILES main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)

add_executable(tool1cd ${SOURCE_FILES})

qt5_use_modules(tool1cd Widgets)
