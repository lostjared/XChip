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

#ifndef _XCHIP_BASETRAITS_H_
#define _XCHIP_BASETRAITS_H_
#include "StdintDef.h"

namespace xchip { namespace utility {
	
// basic metaprogramming return types: true, false
struct true_type { static constexpr bool value = true; };
struct false_type { static constexpr bool value = false; };

// type_is: return the given type itself
template<class T>
struct type_is { using type = T; };




// is same
template<class T, class U>
struct is_same : false_type {};
template<class T>
struct is_same<T, T> : true_type {};

// C++14 variable template
//template<class T, class U>
//constexpr bool is_same_v = is_same<T,U>::value;



// is pointer
template<class T>
struct is_pointer : false_type {};
template<class T>
struct is_pointer<T*> : true_type {};

// C++14 variable template
//template<class T>
//constexpr bool is_pointer_v = is_pointer<T>::value;


// is reference
template<class T>
struct is_reference : false_type {};
template<class T>
struct is_reference<T&> : true_type {};
template<class T>
struct is_reference<T&&> : true_type {};

// C++14 variable template
//template<class T>
//constexpr bool is_reference_v = is_reference<T>::value;


// remove pointer
template<class T>
struct remove_pointer : type_is<T> {} ;
template<class T>
struct remove_pointer<T*> : remove_pointer<T> {};

template<class T>
using remove_pointer_t = typename remove_pointer<T>::type;


// remove const
template<class T>
struct remove_const : type_is<T> {};
template<class T>
struct remove_const<const T> : remove_const<T> {};
template<class T>
using remove_const_t = typename remove_const<T>::type;


// remove volatile
template<class T>
struct remove_volatile : type_is<T> {};
template<class T>
struct remove_volatile<volatile T> : remove_volatile<T> {};
template<class T>
using remove_volatile_t = typename remove_volatile<T>::type;


//remove reference
template<class T>
struct remove_reference : type_is<T> {};
template<class T>
struct remove_reference<T&> : remove_reference<T> {};
template<class T>
struct remove_reference<T&&> : remove_reference<T> {};

template<class T>
using remove_reference_t = typename remove_reference<T>::type;




// remove cv
template<class T>
struct remove_cv : type_is< remove_volatile_t < remove_const_t<T> > > {};
template<class T>
using remove_cv_t = typename remove_cv<T>::type;

// remove cvr
template<class T>
struct remove_cvr : type_is< remove_volatile_t < remove_const_t < remove_reference_t<T> > > > {};
template<class T>
using remove_cvr_t = typename remove_cvr<T>::type;


// remove array
template<class T>
struct remove_array : type_is<T> {};
template<class T, size_t sz>
struct remove_array<T[sz]> : remove_array<T> {};
template<class T>
using remove_array_t = typename remove_array<T>::type;


// remove all
template<class T>
struct remove_all : type_is< remove_cv_t < remove_pointer_t < remove_reference_t< remove_array_t< remove_cv_t<T> > > > > > {};


template<class T>
using remove_all_t = typename remove_all<T>::type;


// enable_if
template<bool cond, class T>
struct enable_if : type_is<T> {};

template<class T>
struct enable_if<false, T> {};


template<bool cond, class T>
using enable_if_t = typename enable_if<cond, T>::type;


// conditional
template<bool cond, class T, class F>
struct conditional : type_is<T> {};

template<class T, class F>
struct conditional<false, T, F> : type_is<F> {};

template<bool cond, class T, class F>
using conditional_t = typename conditional<cond, T, F>::type;







// is numeric
template<class T>
struct is_numeric : 
	conditional_t< is_same<uint8_t, remove_cvr_t<T>>::value
                         || is_same<int8_t, remove_cvr_t<T>>::value
                         || is_same<uint16_t, remove_cvr_t<T>>::value
                         || is_same<int16_t, remove_cvr_t<T>>::value
                         || is_same<uint32_t, remove_cvr_t<T>>::value
                         || is_same<int32_t, remove_cvr_t<T>>::value
                         || is_same<uint64_t, remove_cvr_t<T>>::value
                         || is_same<int64_t, remove_cvr_t<T>>::value
                         || is_same<long, remove_cvr_t<T>>::value
                         || is_same<unsigned long, remove_cvr_t<T>>::value
                         || is_same<long long, remove_cvr_t<T>>::value
                         || is_same<unsigned long long, remove_cvr_t<T>>::value
                         || is_same<size_t, remove_cvr_t<T>>::value
                         || is_same<float, remove_cvr_t<T>>::value
                         || is_same<double, remove_cvr_t<T>>::value
                         || is_same<long double, remove_cvr_t<T>>::value, true_type, false_type> 
{



};





}}


#endif
