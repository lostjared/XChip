#include <cstdlib>
#include <XChip/Utility/Alloc.h>


 
namespace xchip { namespace utility {



void* alloc_arr(const size_t size) noexcept
{
	auto* const block = (size_t*) std::malloc(size + sizeof(size_t));
	if (!block)
		return nullptr;

	*block = size;
	return block + 1;
}



void* realloc_arr(void* from, const size_t size) noexcept
{
	auto* const block = (size_t*) std::realloc(((size_t*)from-1), size + sizeof(std::size_t));
	if(!block)
		return nullptr;

	*block = size;
	return block + 1;

}




void free_arr(const void* block) noexcept
{
	if (block != nullptr)
		std::free(((std::size_t*)block) - 1);
}






}}
