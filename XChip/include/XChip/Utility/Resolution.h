#ifndef _XCHIP_UTILITY_RESOLUTION_H_
#define _XCHIP_UTILITY_RESOLUTION_H_
#include <iostream>




namespace xchip { namespace utility {



struct Resolution
{
	constexpr Resolution() : w(0), h(0) {}
	constexpr Resolution(int _w, int _h) : w(_w), h(_h) {}

	int w, h;
};



inline std::ostream& operator<<(std::ostream& os, const Resolution& res)
{
	os << res.w << "x" << res.h;
	return os;
}




}}




#endif
