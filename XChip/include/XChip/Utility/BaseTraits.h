#ifndef _XCHIP_BASETRAITS_H_
#define _XCHIP_BASETRAITS_H_


namespace xchip { namespace utility {
	



struct true_type { static constexpr bool value = true; };
struct false_type { static constexpr bool value = false; };
template<class T>
struct type_is { using type = T; };


template<bool cond, class T>
struct enable_if;

template<class T>
struct enable_if<true, T> : type_is<T> {};

template<class T>
struct enable_if<false, T> {};

template<class T, class U>
struct is_same : false_type {};

template<class T>
struct is_same<T, T> : true_type {};


template<class T>
struct is_pointer : false_type {};
template<class T>
struct is_pointer<T*> : true_type {};



template<bool cond, class T>
using enable_if_t = typename enable_if<cond, T>::type;











}}


#endif
