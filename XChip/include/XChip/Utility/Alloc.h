#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H_
#include <cstring>
#include "StdintDef.h"
#include "BaseTraits.h"
#include "Assert.h"

namespace xchip { namespace utility {



extern void* alloc_arr(const size_t bytes) noexcept;

extern void* realloc_arr(void* from, const size_t bytes) noexcept;

extern void free_arr(const void* block) noexcept;
 
extern void arr_zero(void* arr) noexcept;

extern void arr_zero(void* arr, const size_t size) noexcept;


template<class T>
enable_if_t<is_pointer<T>::value && !is_same<remove_all_t<T>, uint8_t>::value,
size_t> arr_size(const T arr) noexcept
{
	ASSERT_MSG(arr != nullptr, "attempt to get size from nullptr");
	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return (*size) / sizeof(remove_all_t<T>);
}

template<class T>
enable_if_t<is_pointer<T>::value && is_same<remove_all_t<T>, uint8_t>::value,
size_t> arr_size(const T arr) noexcept
{
	ASSERT_MSG(arr != nullptr, "attempt to get size from nullptr");
	const auto size = reinterpret_cast<const size_t*>(arr) - 1;
	return *size;

}



template<class T, const size_t sz>
constexpr size_t arr_size(const T(&)[sz]) noexcept
{
	return sz;
}




template<class T, const size_t sz>
void arr_zero(T(&arr)[sz]) noexcept
{
	for (auto& it : arr)
		it = 0;
}















}}
#endif
