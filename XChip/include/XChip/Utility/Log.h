#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <string>
#include "Traits.h"


namespace xchip { namespace utility {
	
namespace literals {
		inline std::string operator"" _s(const char* str, std::size_t) { return std::string(str); }
		template<class N>
		enable_if_t<std::is_arithmetic<N>::value, std::string>
			operator+(std::string&& str, N val)
		{
			return std::move(str += std::to_string(val));
		}
}




inline void CLS()
{
	#ifdef _WIN32
		//	std::system("cls");
	#elif __linux__
			std::system("clear");
	#endif
}


inline void LOG(const std::string& msg)
{
	std::cout << msg << std::endl;
}


inline void LOGerr(const std::string& msg)
{
	std::cerr << msg << std::endl;
}



}}



#endif // LOG_H
