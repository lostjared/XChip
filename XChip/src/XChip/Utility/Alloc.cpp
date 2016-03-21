#include <cstdlib>
#include <XChip/Utility/Alloc.h>



namespace xchip { namespace utility {

static std::size_t* block;

void* alloc_arr(const std::size_t size)
{
	block = (std::size_t*) std::malloc(size + sizeof(std::size_t));
	if (block == nullptr)
		return nullptr;

	*block = size;
	return block + 1;
}


std::size_t get_arr_bytes(const void* arr)
{
	if (arr == nullptr) return 0;

	block = (std::size_t*) arr;
	return *(block - 1);
}






}}
