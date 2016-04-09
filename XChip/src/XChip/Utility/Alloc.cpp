#include <cstdlib>
#include <XChip/Utility/Alloc.h>


 
namespace xchip { namespace utility {



void* alloc_arr(const std::size_t size) noexcept
{
	auto* const block = (std::size_t*) std::malloc(size + sizeof(std::size_t));
	if (block == nullptr)
		return nullptr;

	*block = size;
	return block + 1;
}



void* realloc_arr(void* from, const std::size_t size) noexcept
{
	auto* const block = (std::size_t*) std::realloc(((std::size_t*)from-1), size + sizeof(std::size_t));
	if(from == nullptr)
		return nullptr;

	*block = size;
	return block + 1;

}








}}
