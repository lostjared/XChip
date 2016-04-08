#ifndef _XCHIP_UTILITY_MEMORY_H_
#define _XCHIP_UTILITY_MEMORY_H_
#include <memory>
#include "Traits.h"

namespace xchip { namespace utility {


template<class T, class... Args> 
inline enable_if_t<!std::is_array<T>::value,
	std::unique_ptr<T>> make_unique(Args&&... args) noexcept
{
	return (std::unique_ptr<T>(new(std::nothrow) T(std::forward<Args>(args)...)));
}


template<class T> 
inline enable_if_t<std::is_array<T>::value && std::extent<T>::value == 0,
	std::unique_ptr<T>> make_unique(const std::size_t sz) noexcept
{
	typedef typename std::remove_extent<T>::type elem;
	return (std::unique_ptr<T>(new(std::nothrow) elem[sz]()));
}

template<class T, class... Args>
	enable_if_t<std::extent<T>::value != 0,
	void> make_unique(Args&&...) noexcept = delete;




}}


#endif
