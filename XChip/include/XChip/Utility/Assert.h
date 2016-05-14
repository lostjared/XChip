#ifndef _XCHIP_UTILITY_ASSERT_H_
#define _XCHIP_UTILITY_ASSERT_H_


#if !_DEBUG
// if not debug build, includes nothing.
#define NDEBUG 1
#define ASSERT_MSG(cond, msg)




#else

// else, includes...
#include <cassert>
#include <cstdio>


namespace xchip { namespace utility {
// save space on binary
static void print_assert(const char* msg) noexcept
{
	static const char* ASSERT_FMT_STRING = "ASSERT_MSG: %s!!\n";
	std::fprintf(stderr, ASSERT_FMT_STRING, msg);
}

}}

#define ASSERT_MSG(cond, msg) { if(!(cond)) { xchip::utility::print_assert(msg); assert(cond); } }






#endif
#endif
