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

#ifndef _XCHIP_UTILS_TRAITS_H_
#define _XCHIP_UTILS_TRAITS_H_
#include <type_traits>
#include "BaseTraits.h"

namespace xchip { namespace utils {  

template<class T>
using underlying_type_t = typename std::underlying_type<T>::type;

template<class T> 
constexpr underlying_type_t<T> toUtype(T t) noexcept
{
	return static_cast<underlying_type_t<T>>(t);
}


}}



#endif
