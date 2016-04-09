#ifndef _XCHIP_UTILITY_LOG_H
#define _XCHIP_UTILITY_LOG_H
#include <iostream>
#include <iomanip>
#include <string>

namespace xchip { namespace utility {
	
namespace literals {
	inline std::string operator"" _s(const char* str, std::size_t) noexcept { return std::string(str); }
}



inline void CLS() noexcept
{
	#ifdef _WIN32
			std::system("cls");
	#elif __linux__ | __CYGWIN32__
			std::system("clear");
	#endif
}


enum class Endl { Yes, No };
enum class Fmt { Dec, Hex, Oct };

inline void LOG(const std::string& out) noexcept
{
	std::cout << out << std::endl;
}



inline void LOGerr(const std::string& out) noexcept
{
	std::cerr << out << std::endl;
}



inline void LOG(const std::string& out, Endl endl) noexcept
{
	std::cout << out;
	if (endl == Endl::Yes)
		std::cout << std::endl;
}


inline void LOGerr(const std::string& out, Endl endl) noexcept
{
	std::cerr << out;
	if (endl == Endl::Yes)
		std::cerr << std::endl;
}



template<class N>
inline  void LOG(const N number, Fmt ofmt = Fmt::Dec, Endl endl = Endl::Yes) noexcept
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
inline  void LOGerr(const N number, Fmt ofmt = Fmt::Dec, Endl endl = Endl::Yes) noexcept
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
