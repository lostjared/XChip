#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H
#include <cstddef>
#include <algorithm>


namespace xchip { namespace utility {



extern void* alloc_arr(const std::size_t size);


inline void free_arr(void* block)
{
	std::free(((std::size_t*)block) - 1);
}

extern std::size_t get_arr_bytes(const void* arr);





template<class T>
void clean_arr(T* arr)
{
	if (arr != nullptr)
		std::fill_n(arr, get_arr_size(arr), 0);
}



template<class T>
std::size_t get_arr_size(const T* arr)
{
	if (arr == nullptr) return 0;

	const std::size_t* size = (std::size_t*) arr;
	return *(size - 1) / sizeof(T);
}





}}
#endif
