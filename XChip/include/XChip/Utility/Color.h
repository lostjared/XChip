#ifndef _XCHIP_UTILITY_COLOR_H_
#define _XCHIP_UTILITY_COLOR_H_
#include <iostream>
#include "StdintDef.h"


namespace xchip { namespace utility {


struct Color
{
	constexpr Color() : r(0), g(0), b(0) {};
	constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_) 
		: r(r_), g(g_), b(b_) {};

	uint8_t r;
	uint8_t g;
	uint8_t b;
};


inline std::ostream& operator<<(std::ostream& os, const Color& color)
{	
	os << "R(" << +color.r << ") G(" << +color.g << ") B(" << +color.b << ")";
	return os;
}


}}
























#endif
