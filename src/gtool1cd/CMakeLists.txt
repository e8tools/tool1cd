cmake_minimum_required(VERSION 2.8)
project(gtool1cd)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

set (QHEXEDIT_HEADERS
	QHexEdit/document/commands/hexcommand.h
	QHexEdit/document/commands/insertcommand.h
	QHexEdit/document/commands/removecommand.h
	QHexEdit/document/commands/replacecommand.h
	QHexEdit/document/gapbuffer.h
	QHexEdit/document/qhexcursor.h
	QHexEdit/document/qhexdocument.h
	QHexEdit/document/qhextheme.h
	QHexEdit/paint/qhexmetrics.h
	QHexEdit/paint/qhexpainter.h
	QHexEdit/qhexedit.h
	QHexEdit/qhexeditprivate.h
	QHexEdit/document/metadata/qhexmetadataitem.h
	QHexEdit/document/metadata/qhexmetadata.h
)

set (QHEXEDIT_SOURCES
	QHexEdit/document/commands/hexcommand.cpp
	QHexEdit/document/commands/insertcommand.cpp
	QHexEdit/document/commands/removecommand.cpp
	QHexEdit/document/commands/replacecommand.cpp
	QHexEdit/document/gapbuffer.cpp
	QHexEdit/document/qhexcursor.cpp
	QHexEdit/document/qhexdocument.cpp
	QHexEdit/document/qhextheme.cpp
	QHexEdit/paint/qhexmetrics.cpp
	QHexEdit/paint/qhexpainter.cpp
	QHexEdit/qhexedit.cpp
	QHexEdit/qhexeditprivate.cpp
	QHexEdit/document/metadata/qhexmetadataitem.cpp
	QHexEdit/document/metadata/qhexmetadata.cpp
)


file (GLOB MODEL_HEADERS "models/*.h")
file (GLOB MODEL_SOURCES "models/*.cpp")

set(HEADER_FILES mainwindow.h starter.h littlelogwindow.h cache.h table_fields_window.h table_data_window.h
	skobkatextwindow.h models/tables_list_model.h models/fields_list_model.h models/table_data_model.h
	export_table_to_xml_dialog.h configurations_window.h
	${MODEL_HEADERS} ${QHEXEDIT_HEADERS} BlobViewer/blob_viewer.h container_form.h)
set(UI_FILES mainwindow.ui starter.ui littlelogwindow.ui table_fields_window.ui table_data_window.ui
	skobkatextwindow.ui export_table_to_xml_dialog.ui configurations_window.ui BlobViewer/blob_viewer.ui
	container_form.ui)
set(SOURCE_FILES main.cpp mainwindow.cpp starter.cpp littlelogwindow.cpp cache.cpp table_fields_window.cpp table_data_window.cpp
	skobkatextwindow.cpp export_table_to_xml_dialog.cpp configurations_window.cpp
	${MODEL_SOURCES} ${QHEXEDIT_SOURCES} BlobViewer/blob_viewer.cpp container_form.cpp)

set (RESOURCES gtool1cd.rc gtool1cd.qrc)

if (MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows /ENTRY:mainCRTStartup")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /subsystem:windows /ENTRY:mainCRTStartup")
endif()

add_executable(gtool1cd WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${RESOURCES})

include_directories (${SOURCE_DIR}/tool1cd)
include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (gtool1cd tool1cd)


find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (gtool1cd ${ZLIB_LIBRARIES})

install (TARGETS gtool1cd DESTINATION bin)

qt5_use_modules(gtool1cd Widgets)
