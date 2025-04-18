#INCLUDE_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------------
# Define jsoncpp Library
#-----------------------------------------------------------------------------

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()

set (JSONCPP_HEADERS
	include/json/allocator.h
	include/json/assertions.h
	include/json/autolink.h
	include/json/config.h
	include/json/features.h
	include/json/forwards.h
	include/json/json.h
	include/json/reader.h
	include/json/value.h
	include/json/version.h
	include/json/writer.h
	src/lib_json/json_tool.h
	src/lib_json/json_valueiterator.inl
)

SET (JSONCPP_SRCS
	src/lib_json/json_reader.cpp
	src/lib_json/json_value.cpp
	src/lib_json/json_writer.cpp
)

source_group("Header Files" FILES ${JSONCPP_HEADERS})
source_group("Source Files" FILES ${JSONCPP_SRCS})

ADD_LIBRARY (jsoncpp STATIC ${JSONCPP_SRCS} ${JSONCPP_HEADERS})
set(COM_LIBS ${COM_LIBS} jsoncpp CACHE INTERNAL "com_lib")
set_target_properties (jsoncpp PROPERTIES
    FOLDER third-part
)

install(TARGETS jsoncpp
    CONFIGURATIONS Release
    RUNTIME DESTINATION bin/Release
    LIBRARY DESTINATION lib/Release
    ARCHIVE DESTINATION lib/Release
)

install(TARGETS jsoncpp
    CONFIGURATIONS Debug
    RUNTIME DESTINATION bin/Debug
    LIBRARY DESTINATION lib/Debug
    ARCHIVE DESTINATION lib/Debug
)
