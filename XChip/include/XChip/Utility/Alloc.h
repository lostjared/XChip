#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H_
#include <cstdint>
#include <cstddef>
#include <algorithm>



namespace xchip { namespace utility {


extern void* alloc_arr(const std::size_t size) noexcept;




inline void free_arr(const void* block) noexcept
{
	if (block != nullptr)
		std::free(((std::size_t*)block) - 1);
}




template<class T>
std::size_t get_arr_size(const T* arr) noexcept
{
	if (arr == nullptr) return 0;
	const std::size_t* const size = (std::size_t*) arr;
	return *(size - 1) / sizeof(T);
}





template<>
inline std::size_t get_arr_size<uint8_t>(const uint8_t* arr) noexcept
{
	if (arr == nullptr) return 0;
	const std::size_t* const size = (std::size_t*) arr;
	return *(size - 1);
}





template<class T>
void arr_zero(T* arr) noexcept
{
	if (arr != nullptr)
		std::fill_n(arr, get_arr_size(arr), 0);
}









}}
#endif
