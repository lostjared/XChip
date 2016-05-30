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

#ifndef _XCHIP_UTILITY_LOG_H
#define _XCHIP_UTILITY_LOG_H
#include <iostream>
#include <iomanip>
#include <string>
#include "BaseTraits.h"

namespace xchip { namespace utility {
	
namespace literals 
{
	inline std::string operator"" _s(const char* str, std::size_t) noexcept { return std::string(str); }
}



inline void CLS() noexcept
{
	#if defined(_WIN32)
			std::system("cls");
	#elif defined(__linux__) | defined(__CYGWIN32__) | defined(__APPLE__)
			std::system("clear");
	#endif
}


enum class Endl { Yes, No };
enum class Fmt { Dec, Hex, Oct };



template<class T>
enable_if_t<!is_numeric<T>::value, 
void> LOG(const T& out) noexcept
{
	std::cout << out << std::endl;
}


template<class T>
enable_if_t<!is_numeric<T>::value, 
void> LOGerr(const T& out) noexcept
{
	std::cerr << out << std::endl;
}



template<class T>
enable_if_t<!is_numeric<T>::value, 
void> LOG(const T& out, Endl endl) noexcept
{
	std::cout << out;
	if (endl == Endl::Yes)
		std::cout << std::endl;
}


template<class T>
enable_if_t<!is_numeric<T>::value, 
void> LOGerr(const T& out, Endl endl) noexcept
{
	std::cerr << out;
	if (endl == Endl::Yes)
		std::cerr << std::endl;
}



template<class N>
enable_if_t<is_numeric<N>::value, 
void> LOG(const N number, Fmt ofmt = Fmt::Dec, Endl endl = Endl::Yes) noexcept
{

	if (ofmt != Fmt::Dec) {
		const auto oldf = std::cout.setf((ofmt == Fmt::Hex) ? std::ios::hex : std::ios::oct,
			                             std::ios::basefield);

		std::cout << std::showbase << number;
		std::cout.setf(oldf);
	}
	else {
		std::cout << number;
	}

	if (endl == Endl::Yes)
		std::cout << std::endl;
}





template<class N>
enable_if_t<is_numeric<N>::value, 
void> LOGerr(const N number, Fmt ofmt = Fmt::Dec, Endl endl = Endl::Yes) noexcept
{
	
	if (ofmt != Fmt::Dec) {
		const auto oldf = std::cerr.setf((ofmt==Fmt::Hex) ? std::ios::hex : std::ios::oct, 
			                            std::ios::basefield);

		std::cerr << std::showbase << number;
		std::cerr.setf(oldf);
	}
	else {
		std::cerr << number;
	}

	if (endl == Endl::Yes)
		std::cerr << std::endl;
}







}}



#endif // LOG_H
