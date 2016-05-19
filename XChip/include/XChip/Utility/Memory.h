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

#ifndef _XCHIP_UTILITY_MEMORY_H_
#define _XCHIP_UTILITY_MEMORY_H_
#include <memory>
#include "Traits.h"
 
namespace xchip { namespace utility {



template<class T, class... Args>
inline enable_if_t<!std::is_array<T>::value,
	std::unique_ptr<T>> make_unique(Args&&... args)
{
	return (std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
}


template<class T>
inline enable_if_t<std::is_array<T>::value && std::extent<T>::value == 0,
	std::unique_ptr<T>> make_unique(const std::size_t sz)
{
	typedef typename std::remove_extent<T>::type elem;
	return (std::unique_ptr<T>(new elem[sz]()));
}

template<class T, class... Args>
enable_if_t<std::extent<T>::value != 0,
	void> make_unique(Args&&...) = delete;






}}


#endif
