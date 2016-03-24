# xchip test


project(XChip.exe)
cmake_minimum_required(VERSION 2.8)

#additional libs, subdirectories libs can use this variable
set(ADD_LIBS "" CACHE STRING "additional libs to link...")

#sub libs
add_subdirectory(src/SDL)
add_subdirectory(src/XChip)



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
file(GLOB_RECURSE APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/Test/*.cpp)
file(GLOB_RECURSE APP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)


# Linkar src
add_executable(${PROJECT_NAME} ${APP_HEADERS} ${APP_SRC})

message(STATUS "Additional libs: " ${ADD_LIBS})

# Link libs
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${ADD_LIBS} SDL_MEDIA XChip )
