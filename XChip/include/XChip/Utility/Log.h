#ifndef _XCHIP_UTILITY_LOG_H
#define _XCHIP_UTILITY_LOG_H
#include <iostream>
#include <string>
#include "Traits.h"

 
namespace xchip { namespace utility {
	
namespace literals {
		
	inline std::string operator"" _s(const char* str, std::size_t) { return std::string(str); }
	template<class N>
	constexpr enable_if_t<std::is_arithmetic<N>::value, std::string>
		operator+(std::string&& str, const N val) noexcept 
	{
		return std::move(str += std::to_string(val));
	}
}




inline void CLS() noexcept
{
	#ifdef _WIN32
			std::system("cls");
	#elif __linux__ | __CYGWIN32__
			std::system("clear");
	#endif
}

template<class T>
void LOG(const T& msg)
{
	std::cout << msg << std::endl;
}


template<class T>
void LOGerr(const T& msg)
{
	std::cerr << msg << std::endl;
}


}}



#endif // LOG_H
