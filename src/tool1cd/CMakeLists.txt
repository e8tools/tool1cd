cmake_minimum_required (VERSION 2.8)
project (tool1cd)

set (TOOL1CD_SOURCES ParseCommandLine.cpp MessageRegistration.cpp Class_1CD.cpp
	Common.cpp ConfigStorage.cpp Parse_tree.cpp APIcfBase.cpp TempStream.cpp Base64.cpp UZLib.cpp Messenger.cpp
	V8Object.cpp Field.cpp Index.cpp Table.cpp TableFiles.cpp TableFileStream.cpp
	MemBlock.cpp CRC32.cpp Packdata.cpp PackDirectory.cpp)
set (TOOL1CD_HEADERS ParseCommandLine.h MessageRegistration.h Class_1CD.h
	Common.h ConfigStorage.h Parse_tree.h APIcfBase.h TempStream.h Base64.h UZLib.h Messenger.h ErrorCode.h
	db_ver.h NodeTypes.h V8Object.h Constants.h Field.h Index.h Table.h TableFiles.h
	TableFileStream.h MemBlock.h CRC32.h Packdata.h PackDirectory.h)

add_library (tool1cd STATIC ${TOOL1CD_SOURCES} ${TOOL1CD_HEADERS})

include_directories (${SOURCE_DIR}/SystemClasses)
target_link_libraries (tool1cd SystemClasses)

add_definitions (-DBOOST_ALL_NO_LIB)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED OFF)
set (Boost_USE_STATIC_RUNTIME ON)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (tool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (tool1cd ${ZLIB_LIBRARIES})
