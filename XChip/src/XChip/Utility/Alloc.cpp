#include <cstdlib>
#include <XChip/Utility/Alloc.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Assert.h>

 

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
	ASSERT_MSG(from != nullptr,
	"Alloc.cpp::realloc_arr: attempt to realloc from null memory!");

	auto* const block = (size_t*) std::realloc(((size_t*)from-1), size + sizeof(size_t));
	if(!block)
		return nullptr;

	*block = size;
	return block + 1;

}




void free_arr(const void* block) noexcept
{
	ASSERT_MSG(block != nullptr,
        "Alloc.cpp::free_arr: attempt to free null pointer!");

	std::free(((size_t*)block) - 1);
}






}}
