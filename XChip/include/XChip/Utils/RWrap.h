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

#ifndef _XCHIP_UTILS_RWRAP_H_
#define _XCHIP_UTILS_RWRAP_H_

#include "BaseTraits.h"






namespace xchip { namespace utils {




template<class T, class F>
class RWrap
{
	// underlying T's type. 
	// Exemple: if T is int*. UT is int, if T is int**, UT is int*
	using UT = remove_reference_t<type_is_t<decltype(*T())>>;

public:
	RWrap& operator=(const RWrap&) = delete;
	RWrap& operator=(RWrap&&) = delete;
	RWrap(RWrap&) = delete;


	RWrap(RWrap&&) = default;

	constexpr RWrap(F&& f);
	constexpr RWrap(T&& t, F&& f);
	~RWrap();


	const T& data() const;
	T& data();

	constexpr operator T() const;

	template<class V>
	constexpr T operator+(const V val) const;

	template<class V>
	constexpr T operator-(const V val) const;

	template<class V>
	const UT& operator[](const V val) const;
	
	const UT& operator*() const;
	const UT& operator->() const;


	template<class V>
	UT& operator[](const V val);

	template<class V>
	T& operator+=(const V val);

	template<class V>
	T& operator-=(const V val);


	T& operator++();
	T operator++(int);
	T& operator--();
	T operator--(int);

	T& operator=(const T other);
	UT& operator*();
	UT& operator->();

	

private:
	F _f;
	T _t;
};




template<class T, class F>
inline constexpr 
RWrap<T, F>::RWrap(F&& f) 
	: _f(forward<F>(f))
{
	static_assert(noexcept(_f(&_t)), "Destructor's functor must be noexcept");
}

template<class T, class F>
inline constexpr 
RWrap<T, F>::RWrap(T&& t, F&& f) 
	: _f(forward<F>(f)),
	_t(forward<T>(t))
{
	static_assert(noexcept(_f(&_t)), "Destructor's functor must be noexcept");
}

template<class T, class F>
RWrap<T, F>::~RWrap()
{
	_f(&_t);
}


template<class T, class F>
inline T& RWrap<T, F>::data() { return _t; }


template<class T, class F>
inline const T& RWrap<T, F>::data() const { return _t; }



template<class T, class F>
template<class V>
inline constexpr T RWrap<T, F>::operator+(const V val) const { return _t + val; }



template<class T, class F>
template<class V>
inline constexpr T RWrap<T, F>::operator-(const V val) const { return _t - val; }



template<class T, class F>
template<class V>
inline const typename RWrap<T, F>::UT& RWrap<T, F>::operator[](const V val) const { return _t[val];}



template<class T, class F>
inline const typename RWrap<T, F>::UT& RWrap<T, F>::operator*() const { return *_t; }



template<class T, class F>
inline const typename RWrap<T, F>::UT& RWrap<T, F>::operator->() const { return *_t;}




template<class T, class F>
template<class V>
inline typename RWrap<T, F>::UT& RWrap<T, F>::operator[](const V val) { return _t[val]; }




template<class T, class F>
template<class V>
inline T& RWrap<T, F>::operator+=(const V val) { _t += val; return _t; }




template<class T, class F>
template<class V>
inline T& RWrap<T, F>::operator-=(const V val) { _t -= val; return _t; }




template<class T, class F>
inline T& RWrap<T, F>::operator++() { return ++_t; }


template<class T, class F>
inline T RWrap<T, F>::operator++(int)
{ 
	const auto ret = _t; 
	++_t; 
	return ret; 
}


template<class T, class F>
inline T& RWrap<T, F>::operator--() { return --_t; }



template<class T, class F>
inline T RWrap<T, F>::operator--(int)
{ 
	const auto ret = _t; 
	--_t; 
	return ret; 
}



template<class T, class F>
inline T& RWrap<T, F>::operator=(const T other) { _t = other; return _t; }



template<class T, class F>
inline typename RWrap<T, F>::UT& RWrap<T, F>::operator*() { return *_t; }



template<class T, class F>
inline typename RWrap<T, F>::UT& RWrap<T, F>::operator->() { return *_t;}



template<class T, class F>
inline constexpr RWrap<T, F>::operator T() const { return _t; } 











template<class T, class F>
inline constexpr RWrap<T, F> 
	make_rwrap(T&& t, F&& f) { return RWrap<T, F>(forward<T>(t), forward<F>(f)); }

template<class T, class F>
inline constexpr RWrap<T, F> 
	make_rwrap(F&& f) { return RWrap<T, F>(forward<F>(f)); }







}}

#endif
