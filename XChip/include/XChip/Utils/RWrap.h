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


	template<class _T, class _F>
	friend inline constexpr bool operator==(const RWrap<_T, _F>&, const typename RWrap<_T, _F>::UT* const);
	template<class _T, class _F>
	friend inline constexpr bool operator==(const typename RWrap<_T, _F>::UT* const, const RWrap<_T, _F>&);

	template<class _T, class _F>
	friend inline constexpr bool operator!=(const RWrap<_T, _F>&, const typename RWrap<_T, _F>::UT* const);
	template<class _T, class _F>
	friend inline constexpr bool operator!=(const typename RWrap<_T, _F>::UT* const, const RWrap<_T, _F>&);

	template<class _T, class _F>
	friend inline constexpr _T operator+(const RWrap<_T, _F>&, const typename RWrap<_T, _F>::UT* const);
	template<class _T, class _F>
	friend inline constexpr _T operator+(const typename RWrap<_T, _F>::UT* const, const RWrap<_T, _F>&);

	template<class _T, class _F>
	friend inline constexpr _T operator-(const RWrap<_T, _F>&, const typename RWrap<_T, _F>::UT* const);
	template<class _T, class _F>
	friend inline constexpr _T operator-(const typename RWrap<_T, _F>::UT* const, const RWrap<_T, _F>&);



public:
	RWrap& operator=(const RWrap&) = delete;
	RWrap& operator=(RWrap&&) = delete;
	RWrap(RWrap&) = delete;


	
	constexpr RWrap(F&& f);
	constexpr RWrap(T&& t, F&& f);
	RWrap(RWrap&&);
	~RWrap();


	const T& data() const;
	T& data();

	constexpr operator const T() const;
	operator T();

	template<class V>
	const UT& operator[](const V val) const;
	
	const UT& operator*() const;

	const UT* operator->() const;


	template<class V>
	UT& operator[](const V val);
	UT& operator*();
	UT* operator->();


	T& operator=(const T other);

private:
	F _f;
	T _t;
};




template<class T, class F>
inline constexpr 
RWrap<T, F>::RWrap(F&& f) 
	: _f(forward<F>(f))
{
	static_assert(noexcept(_f(_t)), "Destructor's functor must be noexcept");
}

template<class T, class F>
inline constexpr 
RWrap<T, F>::RWrap(T&& t, F&& f) 
	: _f(forward<F>(f)),
	_t(forward<T>(t))
{
	static_assert(noexcept(_f(_t)), "Destructor's functor must be noexcept");
}


template<class T, class F>
inline RWrap<T, F>::RWrap(RWrap&& other)
	: _f(move(other._f)),
	_t(move(other._t))
{
	other._t = nullptr;
}


template<class T, class F>
inline RWrap<T, F>::~RWrap()
{
	if(_t != nullptr)
		_f(_t);
}


template<class T, class F>
inline T& RWrap<T, F>::data() { return _t; }


template<class T, class F>
inline const T& RWrap<T, F>::data() const { return _t; }



template<class T, class F>
inline constexpr RWrap<T, F>::operator const T() const { return _t; } 


template<class T, class F>
inline RWrap<T, F>::operator T() { return _t; } 


template<class T, class F>
template<class V>
inline const typename RWrap<T, F>::UT& RWrap<T, F>::operator[](const V val) const { return _t[val];}


template<class T, class F>
inline const typename RWrap<T, F>::UT& RWrap<T, F>::operator*() const { return *_t; }


template<class T, class F>
inline const typename RWrap<T, F>::UT* RWrap<T, F>::operator->() const { return _t; }

template<class T, class F>
template<class V>
inline typename RWrap<T, F>::UT& RWrap<T, F>::operator[](const V val) { return _t[val]; }

template<class T, class F>
inline typename RWrap<T, F>::UT& RWrap<T, F>::operator*() { return *_t; }

template<class T, class F>
inline typename RWrap<T, F>::UT* RWrap<T, F>::operator->() { return _t;}



template<class T, class F>
inline T& RWrap<T, F>::operator=(const T other) 
{ 
	_f(&_t); 
	_t = other; 
	return _t; 
}


template<class T, class F>
inline constexpr 
bool operator==(const RWrap<T, F>& lhs, 
                 const typename RWrap<T, F>::UT* const rhs) 
{ 
	return lhs._t == rhs; 
}


template<class T, class F>
inline constexpr 
bool operator==(const typename RWrap<T, F>::UT* const lhs, 
                 const RWrap<T, F>& rhs) 
{ 
	return lhs == rhs._t; 
}



template<class T, class F>
inline constexpr 
bool operator!=(const RWrap<T, F>& lhs, 
                 const typename RWrap<T, F>::UT* const rhs) 
{ 
	return lhs._t != rhs; 
}


template<class T, class F>
inline constexpr 
bool operator!=(const typename RWrap<T, F>::UT* const lhs, 
                 const RWrap<T, F>& rhs) 
{ 
	return lhs != rhs._t; 
}



template<class T, class F>
inline constexpr 
T operator+(const RWrap<T, F>& lhs, 
                 const typename RWrap<T, F>::UT* const rhs) 
{ 
	return lhs._t + rhs; 
}


template<class T, class F>
inline constexpr 
T operator+(const typename RWrap<T, F>::UT* const lhs, 
                 const RWrap<T, F>& rhs) 
{ 
	return lhs + rhs._t; 
}


template<class T, class F>
inline constexpr 
T operator-(const RWrap<T, F>& lhs, 
                 const typename RWrap<T, F>::UT* const rhs) 
{ 
	return lhs._t - rhs; 
}


template<class T, class F>
inline constexpr 
T operator-(const typename RWrap<T, F>::UT* const lhs, 
                 const RWrap<T, F>& rhs) 
{ 
	return lhs - rhs._t; 
}










template<class T, class F>
inline constexpr RWrap<T, F> make_rwrap(T&& t, F&& f) { return RWrap<T, F>(forward<T>(t), forward<F>(f)); }
template<class T, class F>
inline constexpr RWrap<T, F> make_rwrap(F&& f) { return RWrap<T, F>(forward<F>(f)); }







}}

#endif
