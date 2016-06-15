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

#ifndef _XCHIP_UTILS_LOG_H
#define _XCHIP_UTILS_LOG_H


#include <string>
#include "Ints.h"

namespace xchip { namespace utils {



namespace literals 
{
	inline std::string operator"" _s(const char* str, size_t) { return std::string(str); }
}


extern void Log(const char* fmtString, ...) noexcept;
extern void LogError(const char* fmtString, ...) noexcept;
extern const std::string& GetLastLogError() noexcept;


















}}



#endif // LOG_H
