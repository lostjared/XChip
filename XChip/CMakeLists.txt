# xchip test


project(XChip)
cmake_minimum_required(VERSION 2.8)


#compiler
set(MEDIA_LIB_1 "" CACHE STRING "media library to use 1")
set(MEDIA_LIB_2 "" CACHE STRING "media library to use 2")
set(MEDIA_LIB_3 "" CACHE STRING "media library to use 3")

set(RENDER_SRC "" CACHE FILEPATH "render src file")
set(INPUT_SRC "" CACHE FILEPATH "input src file")
set(EXTRA_SRC "" CACHE STRING "add extra src that must be compiled")

# CXX_FLAGS
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -Winline  -std=c++11")

#######################################################################################
#######################################################################################

# BUILD TYPES
if (CMAKE_BUILD_TYPE STREQUAL "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")

endif()


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} include/ )

# ACHAR OS .CPP E .H
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/XChip/*.cpp)
file(GLOB_RECURSE TEST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/Test/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)


# Linkar .cpp para este binário (que tem o nome do projeto)
add_executable(${PROJECT_NAME} ${HEADERS} ${SRC} ${RENDER_SRC} ${INPUT_SRC} ${EXTRA_SRC} ${TEST} )

# Linkar bibliotecas dinâmicas
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${MEDIA_LIB_1} ${MEDIA_LIB_2} ${MEDIA_LIB_3} )








