project(SDLPlugins)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Plugins/SDLPlugins)

file(GLOB RENDER_PLUGIN ./SdlRender.cpp)
file(GLOB INPUT_PLUGIN ./SdlInput.cpp)
file(GLOB SOUND_PLUGIN ./SdlSound.cpp)
file(GLOB_RECURSE HEADERS XChip/*.h)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")
add_library(XChipSDLRender SHARED ${HEADERS} ${RENDER_PLUGIN})
add_library(XChipSDLInput SHARED ${HEADERS} ${INPUT_PLUGIN})
add_library(XChipSDLSound SHARED ${HEADERS} ${SOUND_PLUGIN})


target_link_libraries(XChipSDLRender SDL2)
target_link_libraries(XChipSDLInput SDL2)
target_link_libraries(XChipSDLSound SDL2)


INSTALL(TARGETS XChipSDLRender XChipSDLInput XChipSDLSound  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/WXChip/bin/plugins)
 

