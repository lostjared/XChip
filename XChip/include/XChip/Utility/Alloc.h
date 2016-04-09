#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H_
#include "StdintDef.h"
#include <cstring>
#include "BaseTraits.h"



namespace xchip { namespace utility {



extern void* alloc_arr(const size_t bytes) noexcept;


extern void* realloc_arr(void* from, const size_t bytes) noexcept;


extern void free_arr(const void* block) noexcept;
 



template<class T>
enable_if_t<is_pointer<T>::value && !is_same<T,uint8_t*>::value,
size_t> arr_size(const T arr) noexcept
{
	if (arr == nullptr) return 0;
	const size_t* const size = (size_t*) arr;
	return *(size - 1) / sizeof(T);
}




template<class T>
enable_if_t<is_pointer<T>::value && is_same<T,uint8_t*>::value,
size_t> arr_size(const T arr) noexcept
{
	if (arr == nullptr) return 0;
	const size_t* const size = (size_t*) arr;
	return *(size - 1);
}





template<class T, const size_t sz>
constexpr size_t arr_size(const T(&)[sz]) noexcept
{
	return sz;
}





inline void arr_zero(void* arr) noexcept
{
	memset(arr, 0, arr_size(reinterpret_cast<uint8_t*>(arr)));
}








}}
#endif
