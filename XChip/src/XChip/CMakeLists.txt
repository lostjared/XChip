project(XChip)
cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(${XCHIP_DEFAULT_INCLUDE_PATH})

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



file(GLOB_RECURSE XCHIP_SRC ./*.cpp)
file(GLOB_RECURSE XCHIP_HEADERS XChip/*.h)


# xchip added
add_library(${PROJECT_NAME} ${XCHIP_HEADERS} ${XCHIP_SRC})







