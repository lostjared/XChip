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





#ifndef _XCHIP_UTILS_DLOADER_H_
#define _XCHIP_UTILS_DLOADER_H_

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#error "Unknown Plataform utils DLoader"
#endif

#include <string>

namespace xchip { namespace utils {



class DLoader
{
public:
	DLoader(const DLoader&) = delete;
	DLoader& operator=(const DLoader&) = delete;
	DLoader() = default;
	DLoader(DLoader&&) noexcept;
	DLoader& operator=(DLoader&&) noexcept;
	~DLoader();
	void Free() noexcept;
	bool Load(const std::string& dlPath);
	void* GetSymbol(const std::string& symbol);
	void Swap(DLoader& other) noexcept;
private:
#if defined(__linux__) || defined(__APPLE__)
	void* _handle = nullptr;
#elif defined(_WIN32)
	HMODULE _handle = nullptr;
#endif

};





}}


#endif

