#ifndef _XCHIP_UTILITY_VECTOR2_H_
#define _XCHIP_UTILITY_VECTOR2_H_
#include <iostream>




namespace xchip { namespace utility {


template<class T>
struct Vector2
{
	constexpr Vector2() : x(0), y(0) {}
	constexpr Vector2(T _x, T _y) : x(_x), y(_y) {}
	T x, y;
};

using Vec2i = Vector2<int>;
using Vec2u = Vector2<unsigned>;
using Vec2f = Vector2<float>;
using Vec2d = Vector2<double>;




template<class T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v)
{
	os << v.x << ", " << v.y;
	return os;
}




}}




#endif
