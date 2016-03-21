#ifndef _XCHIP_UTILITY_ALLOC_H_
#define _XCHIP_UTILITY_ALLOC_H
#include <cstddef>
#include <cstdlib>



void* operator new(const std::size_t size)
{
	static std::size_t* block;
	while (true)
	{
		block = (std::size_t*) std::malloc(size + sizeof(std::size_t));
		if (block)
			break;

		std::new_handler handler = std::set_new_handler(nullptr);
		if (handler)
			handler();
		else
			return nullptr;
	}

	*block = size;
	return (block + 1);
}


void* operator new[](const std::size_t size)
{
	static std::size_t* block;
	while (true)
	{
		block = (std::size_t*) std::malloc(size + sizeof(std::size_t));
		if (block)
			break;

		std::new_handler handler = std::set_new_handler(nullptr);
		if (handler)
			handler();
		else
			return nullptr;
	}

	*block = size;
	return (block + 1);
}


void operator delete(void* block)
{
	if (block != nullptr)
		free(((std::size_t*)block) - 1);

}

void operator delete[](void* block)
{
	if (block != nullptr)
		free(((std::size_t*)block) - 1);
}


namespace xchip { namespace utility {


template<class T>
std::size_t get_arr_size(const T* arr)
{
	if (arr == nullptr) return 0;

	const std::size_t* size = (std::size_t*) arr;
	return *(size - 1) / sizeof(T);
}

std::size_t get_arr_bytes(const void* arr)
{
	if (arr == nullptr) return 0;

	const std::size_t* size = (std::size_t*) arr;
	return *(size - 1);
}






}}
#endif
