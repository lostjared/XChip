# Install script for directory: /home/dhust/Documents/Github/XChip/XChip/src/Plugins/SDLPlugins

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
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLRender.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLRender.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLInput.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLInput.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLSound.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/Plugins/SDLPlugins/XChipSDLSound.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLRender.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLRender.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLInput.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLInput.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLSound.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/WXChip/bin/plugins/XChipSDLSound.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLRender.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLRender.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLInput.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLInput.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins" TYPE SHARED_LIBRARY FILES "/home/dhust/Documents/Github/XChip/XChip/build/src/Plugins/SDLPlugins/XChipSDLSound.so")
  if(EXISTS "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so"
         OLD_RPATH "/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/dhust/Documents/Github/XChip/XChip/build/Bench/EmuApp/plugins/XChipSDLSound.so")
    endif()
  endif()
endif()

