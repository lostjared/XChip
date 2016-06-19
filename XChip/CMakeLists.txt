cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(XChip)
    
 
     
# sanitizers to check leaks and undefined behavior
option(ADDRESS_SANITIZER OFF)
option(UNDEFINED_SANITIZER OFF)


#set on plugins libraries to build
option(BUILD_SDL_PLUGINS OFF)
option(BUILD_SFML_PLUGINS OFF)


#build Test ?
option(BUILD_TEST OFF)

         
#build EmuApp ?
option(BUILD_EMUAPP OFF)


# build WXChip ?
option(BUILD_WXCHIP OFF)





# compiler settings flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -pedantic")

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected! default to release")
	set(CMAKE_BUILD_TYPE "Release")
endif()



# "Release" full optimization , no debug info.
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections -g0")



# "Debug" full debug information, no optimization, asserts enabled
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -D_DEBUG")


# "Bench" better code generation but keep debug information
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Bench")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g  -DNDEBUG")
endif()


if( ADDRESS_SANITIZER )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if( UNDEFINED_SANITIZER )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()





# build dependencies sources
# Xlib:


# include/link directories
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(XLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Xlib/Xlib/include)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Xlib/Xlib/src)



include_directories(${PROJECT_INCLUDE_DIR} /usr/local/include)
include_directories(XLIB_INCLUDE_DIR)
link_directories(/usr/local/lib)






# finally builds XChip ....
add_subdirectory(${PROJECT_SOURCE_DIR})
