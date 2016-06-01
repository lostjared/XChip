cmake_minimum_required(VERSION 2.6)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(XChip)




set(MEMORY_SANITIZER "OFF" CACHE STRING "-fsanitize=memory")
set(UNDEFINED_SANITIZER "OFF" CACHE STRING "-fsanitize=undefined")

#set on plugins libraries to build
set(BUILD_SDL_PLUGINS "OFF" CACHE STRING "")
set(BUILD_SFML_PLUGINS "OFF" CACHE STRING "")

#build Test ?
set(BUILD_TEST "OFF" CACHE STRING "")

# build WXChip ?
set(BUILD_WXCHIP "OFF" CACHE STRING "")


# compiler settings flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -pedantic")

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected! default to release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fomit-frame-pointer")

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -ggdb -D_DEBUG")

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Bench")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -ggdb -DNDEBUG")
endif()

if(${MEMORY_SANITIZER} STREQUAL "ON")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if(${UNDEFINED_SANITIZER} STREQUAL "ON")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()


set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${PROJECT_INCLUDE_DIR} /usr/local/include) 
link_directories(/usr/local/lib)

add_subdirectory(${PROJECT_SOURCE_DIR})
