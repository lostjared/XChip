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

#include <cstdlib>
#include <XChip/Utility/Alloc.h>
#include <XChip/Utility/Assert.h>

 

namespace xchip { namespace utility {


void* alloc_arr(const size_t size) noexcept
{
	ASSERT_MSG(size > 0, "attempt to alloc array of size 0");

	auto* const block = (size_t*) std::malloc(size + sizeof(size_t));
	
	if (block)
	{
		*block = size;
		return block + 1;
	}
	
	return nullptr;
}



void* realloc_arr(void* from, const size_t size) noexcept
{
	ASSERT_MSG(from != nullptr, "attempt to realloc from null memory!");
	ASSERT_MSG(size > 0, "attempt to realloc to size 0!");

	auto* const block = (size_t*) std::realloc(((size_t*)from-1), size + sizeof(size_t));

	if(block)
	{
		*block = size;
		return block + 1;
	}

	return nullptr;
}









}}
