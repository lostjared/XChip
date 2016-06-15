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

#ifndef _XCHIP_UTILS_VECTOR2_H_
#define _XCHIP_UTILS_VECTOR2_H_
#include <iostream>




namespace xchip { namespace utils {


template<class T>
struct Vector2
{
	T x, y;
	constexpr Vector2() : x(0), y(0) {}
	constexpr Vector2(T _x, T _y) : x(_x), y(_y) {}
	constexpr Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y) {}
	constexpr Vector2 operator+(const T val) const;
	constexpr Vector2 operator-(const T val) const;
	constexpr Vector2 operator*(const T val) const;
	constexpr Vector2 operator/(const T val) const;
	constexpr Vector2 operator+(const Vector2& other) const;
	constexpr Vector2 operator-(const Vector2& other) const;
	constexpr Vector2 operator*(const Vector2& other) const;
	constexpr Vector2 operator/(const Vector2& other) const;
	Vector2& operator=(const T val);
	Vector2& operator+=(const T val);
	Vector2& operator-=(const T val);
	Vector2& operator*=(const T val);
	Vector2& operator/=(const T val);
	Vector2& operator=(const Vector2& other);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(const Vector2& other);
	Vector2& operator/=(const Vector2& other);
	Vector2& operator++();
	Vector2 operator++(int);
	Vector2& operator--();
	Vector2 operator--(int);
};

using Vec2i = Vector2<int>;
using Vec2u = Vector2<unsigned>;
using Vec2f = Vector2<float>;
using Vec2d = Vector2<double>;




template<class T>
constexpr Vector2<T> Vector2<T>::operator+(const T val) const { return { x + val, y + val }; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator-(const T val) const { return { x - val, y - val }; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator*(const T val) const { return {x*val, y*val}; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator/(const T val) const { return {x*val, y*val}; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const { return { x + other.x, y + other.y}; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const { return { x - other.x, y - other.y}; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator*(const Vector2<T>& other) const { return { x * other.x, y * other.y}; }
template<class T>
constexpr Vector2<T> Vector2<T>::operator/(const Vector2<T>& other) const { return { x / other.x, y / other.y}; }



template<class T>
Vector2<T>& Vector2<T>::operator=(const T val)
{
	x = val;
	y = val;
	return *this;

}


template<class T>
Vector2<T>& Vector2<T>::operator+=(const T val)
{
	x += val;
	y += val;
	return *this;

}


template<class T>
Vector2<T>& Vector2<T>::operator-=(const T val)
{
	x -= val;
	y -= val;
	return *this;

}


template<class T>
Vector2<T>& Vector2<T>::operator*=(const T val)
{
	x *= val;
	y *= val;
	return *this;
}


template<class T>
Vector2<T>& Vector2<T>::operator/=(const T val)
{
	x /= val;
	y /= val;
	return *this;
}





template<class T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other)
{
	x = other.x;
	y = other.y;
	return *this;

}





template<class T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;
	return *this;

}


template<class T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

template<class T>
Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

template<class T>
Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}






template<class T>
Vector2<T>& Vector2<T>::operator++()
{
	++x; ++y;
	return *this;
}


template<class T>
Vector2<T> Vector2<T>::operator++(int)
{
	Vector2<T> tmp(x, y);
	++x; ++y;
	return tmp;
}




template<class T>
Vector2<T>& Vector2<T>::operator--()
{
	--x; --y;
	return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator--(int)
{
	Vector2<T> tmp(x, y);
	--x; --y;
	return tmp;
}







template<class T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v)
{
	os << v.x << ", " << v.y;
	return os;
}








}}




#endif
