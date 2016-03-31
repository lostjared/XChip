
#SDL MEDIA INTERFACE FOR XCHIP

project(SDL_MEDIA)
cmake_minimum_required(VERSION 2.8)


# CXX_FLAGS
set(CMAKE_CXX_COMPILER "${CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#######################################################################################
#######################################################################################

# BUILD TYPES
if (CMAKE_BUILD_TYPE STREQUAL "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")

endif()


INCLUDE_DIRECTORIES(${XCHIP_DEFAULT_INCLUDE_PATH})

file(GLOB_RECURSE SDL_MEDIA_SRC *.cpp)
file(GLOB_RECURSE SDL_MEDIA_HEADERS SDL/*.h)


set(SDL2LIB "-lSDL2")
set(ADD_LIBS "${ADD_LIBS};${PROJECT_NAME};${SDL2LIB}" PARENT_SCOPE)

add_library(${PROJECT_NAME} ${SDL_MEDIA_HEADERS} ${SDL_MEDIA_SRC})

