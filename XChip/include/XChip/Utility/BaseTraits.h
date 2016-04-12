#ifndef _XCHIP_BASETRAITS_H_
#define _XCHIP_BASETRAITS_H_


namespace xchip { namespace utility {
	
// basic metaprogramming return types: true, false
struct true_type { static constexpr bool value = true; };
struct false_type { static constexpr bool value = false; };

// is same
template<class T, class U>
struct is_same : false_type {};

template<class T>
struct is_same<T, T> : true_type {};



// is pointer
template<class T>
struct is_pointer : false_type {};

template<class T>
struct is_pointer<T*> : true_type {};




template<class T>
struct type_is { using type = T; };


// remove pointer
template<class T>
struct remove_pointer : type_is<T> {} ;
template<class T>
struct remove_pointer<T*> : remove_pointer<T> {};
template<class T>
using remove_pointer_t = typename remove_pointer<T>::type;


// remove cv
template<class T>
struct remove_cv : type_is<T> { };
template<class T>
struct remove_cv<volatile T> : type_is<T> { };
template<class T>
struct remove_cv<const T>  : type_is<T> { };
template<class T>
struct remove_cv<const volatile T> : type_is<T> { };

template<class T>
using remove_cv_t = typename remove_cv<T>::type;


//remove reference
template<class T>
struct remove_ref : type_is<T> { };
template<class T>
struct remove_ref<T&> : remove_ref<T> { };
template<class T>
struct remove_ref<T&&> : remove_ref<T> { };

template<class T>
using remove_ref_t = typename remove_ref<T>::type;




// remove all
template<class T>
struct remove_all : type_is< remove_cv_t < remove_pointer_t < remove_ref_t <T> > > > { };


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
