#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H_
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include "BaseTraits.h"



namespace xchip { namespace utility {



extern void* alloc_arr(const std::size_t bytes) noexcept;


extern void* realloc_arr(void* from, const std::size_t bytes) noexcept;


inline void free_arr(const void* block) noexcept
{
	if (block != nullptr)
		std::free(((std::size_t*)block) - 1);
}
 



template<class T>
enable_if_t<is_pointer<T>::value && !is_same<T,uint8_t*>::value,
std::size_t> arr_size(const T arr) noexcept
{
	if (arr == nullptr) return 0;
	const std::size_t* const size = (std::size_t*) arr;
	return *(size - 1) / sizeof(T);
}




template<class T>
enable_if_t<is_pointer<T>::value && is_same<T,uint8_t*>::value,
std::size_t> arr_size(const T arr) noexcept
{
	if (arr == nullptr) return 0;
	const std::size_t* const size = (std::size_t*) arr;
	return *(size - 1);
}





template<class T, const std::size_t sz>
constexpr std::size_t arr_size(const T(&)[sz]) noexcept
{
	return sz;
}





template<class T>
void arr_zero(T* arr) noexcept
{
	memset(arr, 0, arr_size((uint8_t*)arr));
}








}}
#endif
