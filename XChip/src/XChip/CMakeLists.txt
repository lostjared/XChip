#XCHIP LIB


project(XChip)
cmake_minimum_required(VERSION 2.8)


# CXX_FLAGS
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -Winline  -std=c++11")

#######################################################################################
#######################################################################################

# BUILD TYPES
if (CMAKE_BUILD_TYPE STREQUAL "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-omit-frame-pointer")

endif()


INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ../../include/ )

# ACHAR OS .CPP E .H
file(GLOB_RECURSE XCHIP_SRC *.cpp)
file(GLOB_RECURSE XCHIP_HEADERS /../../include/*.h)


# xchip added
add_library(${PROJECT_NAME} ${XCHIP_HEADERS} ${XCHIP_SRC})







