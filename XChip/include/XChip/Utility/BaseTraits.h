#ifndef _XCHIP_BASETRAITS_H_
#define _XCHIP_BASETRAITS_H_


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

// C++14 variable tempalte
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


// remove cv
template<class T>
struct remove_cv : type_is<T> {};
template<class T>
struct remove_cv<volatile T> : type_is<T> {};
template<class T>
struct remove_cv<const T>  : type_is<T> {};
template<class T>
struct remove_cv<const volatile T> : type_is<T> {};

template<class T>
using remove_cv_t = typename remove_cv<T>::type;


//remove reference
template<class T>
struct remove_reference : type_is<T> {};
template<class T>
struct remove_reference<T&> : remove_reference<T> {};
template<class T>
struct remove_reference<T&&> : remove_reference<T> {};

template<class T>
using remove_reference_t = typename remove_reference<T>::type;




// remove all
template<class T>
struct remove_all : type_is< remove_cv_t < remove_pointer_t < remove_reference_t <T> > > > {};


template<class T>
using remove_all_t = typename remove_all<T>::type;


// enable_if
template<bool cond, class T>
struct enable_if;

template<class T>
struct enable_if<true, T> : type_is<T> {};

template<class T>
struct enable_if<false, T> {};


template<bool cond, class T>
using enable_if_t = typename enable_if<cond, T>::type;






















}}


#endif
