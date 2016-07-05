# Install script for directory: /home/dhust/Documents/Github/XChip/XChip/src/EmuApp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Bench")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp" TYPE EXECUTABLE FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/EmuApp/EmuApp")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/EmuApp")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin" TYPE EXECUTABLE FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/EmuApp/EmuApp")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/EmuApp")
    endif()
  endif()
endif()

