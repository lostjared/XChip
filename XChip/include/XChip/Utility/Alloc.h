#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H_
#include <cstring>
#include "StdintDef.h"
#include "BaseTraits.h"
#include "Assert.h"
#include <type_traits>

namespace xchip { namespace utility {



extern void* alloc_arr(const size_t bytes) noexcept;


extern void* realloc_arr(void* from, const size_t bytes) noexcept;


extern void free_arr(const void* block) noexcept;
 



template<class T>
enable_if_t<is_pointer<T>::value && !is_same<remove_all_t<T>, uint8_t>::value,
size_t> arr_size(const T arr) noexcept
{
	ASSERT_MSG(arr != nullptr,
		"Alloc.h::arr_size: attempt to get size from nullptr");

	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return (*size) / sizeof(T);
}


inline size_t arr_size(const uint8_t* arr) noexcept
{
	ASSERT_MSG(arr != nullptr,
		"Alloc.h::arr_size: attempt to get size from nullptr");
	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return *size;

}



template<class T, const size_t sz>
constexpr size_t arr_size(const T(&)[sz]) noexcept
{
	return sz;
}




template<class T>
enable_if_t<is_pointer<T>::value && !is_same<remove_all_t<T>, uint8_t>::value,
void> arr_zero(T arr) noexcept
{
	ASSERT_MSG(arr != nullptr,
		"Alloc.h::arr_zero: attempt to clean nullptr");
	memset(arr, 0, arr_size(arr) * sizeof(T));
}


inline void arr_zero(uint8_t* arr) noexcept
{
	ASSERT_MSG(arr != nullptr,
		"Alloc.h::arr_zero: attempt to clean nullptr");
	
	memset(arr, 0, arr_size(arr));
}



template<class T, const size_t sz>
void arr_zero(T(&arr)[sz]) noexcept
{
	for (auto& it : arr)
		it = 0;
}







}}
#endif
