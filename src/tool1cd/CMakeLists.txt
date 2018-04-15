cmake_minimum_required (VERSION 2.8)
project (tool1cd)

include_directories(../SystemClasses)
set (SYSTEMCLASSES_SOURCES ../SystemClasses/String.cpp
		../SystemClasses/System.Classes.cpp
		../SystemClasses/System.cpp
		../SystemClasses/System.IOUtils.cpp
		../SystemClasses/TFileStream.cpp
		../SystemClasses/TMemoryStream.cpp
		../SystemClasses/TStream.cpp
		../SystemClasses/TStreamReader.cpp
		../SystemClasses/TStreamWriter.cpp
		../SystemClasses/System.SysUtils.cpp
		../SystemClasses/GetTickCount.cpp)
set (SYSTEMCLASSES_HEADERS ../SystemClasses/String.hpp
		../SystemClasses/System.Classes.hpp
		../SystemClasses/System.hpp
		../SystemClasses/System.IOUtils.hpp
		../SystemClasses/TFileStream.hpp
		../SystemClasses/TMemoryStream.hpp
		../SystemClasses/TStream.hpp
		../SystemClasses/TStreamReader.hpp
		../SystemClasses/TStreamWriter.hpp
		../SystemClasses/System.SysUtils.hpp
		../SystemClasses/GetTickCount.hpp
		../SystemClasses/Exception.hpp)


set (TOOL1CD_SOURCES MessageRegistration.cpp Class_1CD.cpp
	Common.cpp ConfigStorage.cpp Parse_tree.cpp TempStream.cpp Base64.cpp UZLib.cpp Messenger.cpp
	V8Object.cpp Field.cpp Index.cpp Table.cpp TableFiles.cpp TableFileStream.cpp
	MemBlock.cpp CRC32.cpp Packdata.cpp PackDirectory.cpp FieldType.cpp DetailedException.cpp
	BinaryDecimalNumber.cpp save_depot_config.cpp save_part_depot_config.cpp
	SupplierConfig.cpp TableRecord.cpp BinaryGuid.cpp TableIterator.cpp SupplierConfigBuilder.cpp
	main.cpp)

set (TOOL1CD_HEADERS MessageRegistration.h Class_1CD.h
	Common.h ConfigStorage.h Parse_tree.h TempStream.h Base64.h UZLib.h Messenger.h
	db_ver.h NodeTypes.h V8Object.h Constants.h Field.h Index.h Table.h TableFiles.h
	TableFileStream.h MemBlock.h CRC32.h Packdata.h PackDirectory.h FieldType.h DetailedException.h
	BinaryDecimalNumber.h SupplierConfig.h TableRecord.h BinaryGuid.h TableIterator.h SupplierConfigBuilder.h)

# .CF API
set (TOOL1CD_SOURCES ${TOOL1CD_SOURCES} cfapi/V8File.cpp cfapi/V8Catalog.cpp cfapi/TV8FileStream.cpp
	cfapi/APIcfBase.cpp cfapi/V8Time.cpp)
set (TOOL1CD_HEADERS ${TOOL1CD_HEADERS} cfapi/V8File.h cfapi/V8Catalog.h cfapi/TV8FileStream.h
	cfapi/APIcfBase.h cfapi/V8Time.h)

set (TOOL1CD_SOURCES ${TOOL1CD_SOURCES} ${SYSTEMCLASSES_SOURCES})
set (TOOL1CD_HEADERS ${TOOL1CD_HEADERS} ${SYSTEMCLASSES_HEADERS})

if (NOGUI)
	add_library (tool1cd STATIC ${TOOL1CD_SOURCES} ${TOOL1CD_HEADERS})
else()
	add_library (tool1cd SHARED ${TOOL1CD_SOURCES} ${TOOL1CD_HEADERS})
endif()

set_target_properties(tool1cd PROPERTIES LINKER_LANGUAGE CXX)

find_package (Boost 1.53 REQUIRED COMPONENTS filesystem regex system)
include_directories (${Boost_INCLUDE_DIRS})
target_link_libraries (tool1cd ${Boost_LIBRARIES})

find_package (ZLIB REQUIRED)
include_directories (${ZLIB_INCLUDE_DIRS})
target_link_libraries (tool1cd ${ZLIB_LIBRARIES})

if (NOT NOGUI)
	install (TARGETS tool1cd DESTINATION lib)
endif()
