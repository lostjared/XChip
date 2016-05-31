/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#ifndef _XCHIP_UTILITY_ASSERT_H_
#define _XCHIP_UTILITY_ASSERT_H_


#if !_DEBUG
// if not debug build, includes nothing.
#define NDEBUG 1
#define ASSERT_MSG(cond, msg)




#else

// else, includes...
#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#endif

#include <cassert>
#include "Log.h"

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#define _XCHIP_MACRO_STR_EX_(x) #x
#define _XCHIP_MACRO_STR_(x) _XCHIP_MACRO_STR_EX_(x)

#define ASSERT_MSG(cond, msg)                                            \
{                                                                        \
    using xchip::utility::LogError;                                      \
    if(!(cond))                                                          \
    {                                                                    \
        LogError("\n                          \n"                        \
                 "****************************\n"                        \
                 "*                          *\n"                        \
                 "*    !ASSERTION FAILED!    *\n"                        \
                 "*                          *\n"                        \
                 "****************************");                        \
        LogError("CONDITION: "#cond"\n"                                  \
                 "MESSAGE:   " msg "\n"                                  \
                 "FILE: " __FILE__ ":" _XCHIP_MACRO_STR_(__LINE__)"\n"); \
        DEBUG_BREAK();                                                   \
    }                                                                    \
}





#endif
#endif
