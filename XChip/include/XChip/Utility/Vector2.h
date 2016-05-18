#ifndef _XCHIP_UTILITY_VECTOR2_H_
#define _XCHIP_UTILITY_VECTOR2_H_
#include <iostream>




namespace xchip { namespace utility {


template<class T>
struct Vector2
{
	T x, y;
	constexpr Vector2() : x(0), y(0) {}
	constexpr Vector2(T _x, T _y) : x(_x), y(_y) {}
	constexpr Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y) {}
	Vector2 operator+(const T val) const;
	Vector2 operator-(const T val) const;
	Vector2 operator*(const T val) const;
	Vector2 operator/(const T val) const;
	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const Vector2& other) const;
	Vector2 operator/(const Vector2& other) const;
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
Vector2<T> Vector2<T>::operator+(const T val) const { return { x + val, y + val }; }
template<class T>
Vector2<T> Vector2<T>::operator-(const T val) const { return { x - val, y - val }; }
template<class T>
Vector2<T> Vector2<T>::operator*(const T val) const { return {x*val, y*val}; }
template<class T>
Vector2<T> Vector2<T>::operator/(const T val) const { return {x*val, y*val}; }
template<class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const { return { x + other.x, y + other.y}; }
template<class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const { return { x - other.x, y - other.y}; }
template<class T>
Vector2<T> Vector2<T>::operator*(const Vector2<T>& other) const { return { x * other.x, y * other.y}; }
template<class T>
Vector2<T> Vector2<T>::operator/(const Vector2<T>& other) const { return { x / other.x, y / other.y}; }



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
