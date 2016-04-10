#ifndef _XCHIP_UTILITY_TRAITS_H_
#define _XCHIP_UTILITY_TRAITS_H_
#include <type_traits>
#include "BaseTraits.h"

namespace xchip { namespace utility {  

template<class T>
using underlying_type_t = typename std::underlying_type<T>::type;

template<class T> 
constexpr underlying_type_t<T> toUtype(T t) noexcept
{
	return static_cast<underlying_type_t<T>>(t);
}


} }



#endif
