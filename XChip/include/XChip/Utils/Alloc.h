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


#ifndef _XCHIP_UTILS_ALLOC_H_
#define _XCHIP_UTILS_ALLOC_H_
#include <cstdlib>
#include <cstring>
#include "Ints.h"
#include "BaseTraits.h"
#include "Assert.h"

namespace xchip { namespace utils {



extern void* alloc_arr(const size_t bytes) noexcept;

extern void* realloc_arr(void* from, const size_t bytes) noexcept;


inline void free_arr(const void* block)
{
	ASSERT_MSG(block != nullptr, "attempt to free null pointer!");
	std::free(((size_t*)block) - 1);
}





template<class T>
inline enable_if_t<is_pointer<T>::value && !is_same<remove_all_t<T>, uint8_t>::value,
size_t> arr_size(const T arr)
{
	if(!arr)
		return 0;

	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return (*size) / sizeof(remove_all_t<T>);
}


template<class T>
inline enable_if_t<is_pointer<T>::value && is_same<remove_all_t<T>, uint8_t>::value,
size_t> arr_size(const T arr)
{
	if(!arr)
		return 0;

	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return *size;

}



template<class T, const size_t sz>
constexpr size_t arr_size(const T(&)[sz])
{
	return sz;
}




template<class T, const size_t sz>
inline void arr_zero(T(&arr)[sz])
{
	for (auto& it : arr)
		it = 0;
}



inline void arr_zero(void* arr)
{
	ASSERT_MSG(arr != nullptr, "attempt to clean nullptr");
	memset(arr, 0, arr_size(reinterpret_cast<uint8_t*>(arr)));
}


inline void arr_zero(void* arr, const size_t size)
{
	ASSERT_MSG(arr != nullptr, "attempt to clean nullptr");
	memset(arr, 0, size);
}











}}
#endif
