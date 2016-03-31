# xchip test
set(CMAKE_LEGACY_CYGWIN_WIN32 0) 

project(XChipApp)
cmake_minimum_required(VERSION 2.8)


#media variables
set(USING_SDL_MEDIA  "OFF" CACHE STRING "" )

#additional libs, subdirectories libs can use this variable
set(ADD_LIBS "" CACHE STRING "additional libs to link...")




#include paths
set(XCHIP_DEFAULT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE STRING "")
INCLUDE_DIRECTORIES(${XCHIP_DEFAULT_INCLUDE_PATH})



#compiler
set(CXX_COMPILER "clang++")
set(CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -Winline  -std=c++11 -fno-exceptions")



#sub libs
add_subdirectory(src/XChip)

if( ${USING_SDL_MEDIA} )
	add_subdirectory(src/SDL)
endif()


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



# ACHAR OS .CPP E .H
file(GLOB_RECURSE APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/Test/*.cpp)
file(GLOB_RECURSE APP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)


# Linkar src
add_executable(${PROJECT_NAME} ${APP_HEADERS} ${APP_SRC})

message(STATUS "Additional libs: " ${ADD_LIBS})
message(STATUS "XChip default include path: " ${XCHIP_DEFAULT_INCLUDE_PATH})

# Link libs
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${ADD_LIBS} XChip)
