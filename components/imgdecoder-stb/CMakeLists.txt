
set(imgdecoder-stb_header
	imgdecoder-stb.h
	${PROJECT_SOURCE_DIR}/third-part/stb/stb_image.h
    ${PROJECT_SOURCE_DIR}/third-part/pbl_aupng/include/upng.h
)

set(imgdecoder-stb_src
    imgdecoder-stb.cpp
)

source_group("Header Files" FILES ${imgdecoder-stb_header})
source_group("Source Files" FILES ${imgdecoder-stb_src})

if (NOT ENABLE_SOUI_COM_LIB)
    set (imgdecoder-stb_src  ${imgdecoder-stb_src} imgdecoder-stb.rc)
    add_library(imgdecoder-stb SHARED ${imgdecoder-stb_src} ${imgdecoder-stb_header})
    target_link_libraries(imgdecoder-stb utilities4 aupng)
else()
    add_library(imgdecoder-stb STATIC ${imgdecoder-stb_src} ${imgdecoder-stb_header})
    target_compile_definitions(imgdecoder-stb
      PUBLIC SOUI_IMAGE_DECODER_USE_STB_STATIC
    )
endif()

target_compile_definitions(imgdecoder-stb
  PUBLIC SOUI_IMAGE_DECODER_USE_STB
)
target_include_directories(imgdecoder-stb
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${PROJECT_SOURCE_DIR}/utilities/include
  PRIVATE ${PROJECT_SOURCE_DIR}/SOUI/include
  PRIVATE ${PROJECT_SOURCE_DIR}/third-part/stb
  PRIVATE ${PROJECT_SOURCE_DIR}/third-part/pbl_aupng/include
)

set(COM_LIBS ${COM_LIBS} imgdecoder-stb CACHE INTERNAL "com_lib")
set_target_properties (imgdecoder-stb PROPERTIES
    FOLDER components/imgdecoder
)

install(TARGETS imgdecoder-stb
    CONFIGURATIONS Release
    RUNTIME DESTINATION bin/Release
    LIBRARY DESTINATION lib/Release
    ARCHIVE DESTINATION lib/Release
)

install(TARGETS imgdecoder-stb
    CONFIGURATIONS Debug
    RUNTIME DESTINATION bin/Debug
    LIBRARY DESTINATION lib/Debug
    ARCHIVE DESTINATION lib/Debug
)
