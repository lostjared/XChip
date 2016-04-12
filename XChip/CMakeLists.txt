# xchip test
set(CMAKE_LEGACY_CYGWIN_WIN32 0) 

project(XChipApp) 
cmake_minimum_required(VERSION 2.8)


##############################################################
##############################################################
# COMPILER SETTINGS

#include paths
set(XCHIP_DEFAULT_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE STRING "")
INCLUDE_DIRECTORIES(${XCHIP_DEFAULT_INCLUDE_PATH})


#flags && compiler selection
set(CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -pedantic -fno-exceptions -fno-unwind-tables")


if(APPLE)
	set(CXX_COMPILER "clang++")
elseif(CYGWIN)
	set(CXX_COMPILER "clang++")
elseif(UNIX)
	set(CXX_COMPILER "clang++")


endif()



# BUILD TYPES
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CXX_FLAGS "${CXX_FLAGS} -O3")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CXX_FLAGS "${CXX_FLAGS} -O0 -g -ggdb -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -D_DEBUG")

elseif(CMAKE_BUILD_TYPE STREQUAL "OPTDebug")
	set(CXX_FLAGS "${CXX_FLAGS} -O2 -g -ggdb -fno-omit-frame-pointer -D_DEBUG")

endif()

include_directories(/usr/local/include)
link_directories(/usr/local/lib)


#################################################################
################################################################
#Sub Dir Projects settings

#additional libs, subdirectories libs can use this variable
#to add their dependencies to link with the project
set(ADD_LIBS "" CACHE STRING "additional libs to link...")


#turn ON the media library that the APP will be using
set(USING_SDL_MEDIA  "ON" CACHE STRING "" )



#add sub directories libs
#XChip
add_subdirectory(src/XChip)



# SDL_MEDIA if using_sdl_media is on
if( ${USING_SDL_MEDIA} )
	add_subdirectory(src/SDL_MEDIA)
endif()



########################################################
#######################################################
# print the config
message(STATUS "Using Compiler: " ${CXX_COMPILER})
message(STATUS "Flags: " ${CXX_FLAGS})
message(STATUS "Additional libs: " ${ADD_LIBS})
message(STATUS "XChip default include path: " ${XCHIP_DEFAULT_INCLUDE_PATH})



#######################################################################################
#######################################################################################


# set the CMAKE_CXX_...
set(CMAKE_CXX_COMPILER "${CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")



# BUILD TEST
file(GLOB_RECURSE APP_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/Test/*.cpp)
file(GLOB_RECURSE APP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)


# link 
add_executable(${PROJECT_NAME} ${APP_HEADERS} ${APP_SRC})


# Link libs
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${ADD_LIBS} XChip)
