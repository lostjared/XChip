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

#include <cstdio>
#include <stdarg.h>
#include <XChip/Utility/Log.h>







namespace xchip { namespace utility {


static std::string errstr;

void Log(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);
	std::vfprintf(stdout, fmtString, args);
	std::fprintf(stdout, "\n");
	va_end(args);
}


void LogError(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);
	
	errstr.clear();
	errstr.resize(strlen(fmtString) + 400);
	std::vsprintf(&errstr[0], fmtString, args);
	std::fprintf(stderr, errstr.c_str());
	std::fprintf(stderr, "\n");
	
	va_end(args);
}

extern const std::string& GetLastLogError() noexcept
{
	return errstr;
}







}}
