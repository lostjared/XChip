#ifndef _XCHIP_UTILITY_ASSERT_H_
#define _XCHIP_UTILITY_ASSERT_H_


#if !_DEBUG
// if not debug build, includes nothing.
#define NDEBUG 1
#define ASSERT_MSG(...)


#else
// else, includes...
#include <cassert>
#include <cstdio>


namespace xchip { namespace utility {

// save space on binary
static void print_assert(const char* msg)
{
	static const char* ASSERT_MSG_STR = "ASSERT_MSG";
	static const char* ASSERT_MSG_NEWLINE = "\n";
	std::fprintf(stderr, "%s %s %s", ASSERT_MSG_STR, msg, ASSERT_MSG_NEWLINE);
};

}}

#define ASSERT_MSG(cond, msg) { if(!(cond)) { xchip::utility::print_assert(msg); assert(cond); } }






#endif
#endif
