#ifndef _XCHIP_UTILITY_ASSERT_H_
#define _XCHIP_UTILITY_ASSERT_H_
// #define NDEBUG
#include <cassert>

#ifndef NDEBUG
#define ASSERT_MSG(cond, msg) { if(!(cond)) {xchip::utility::LOGerr(msg); assert(false); } }
#else
#define ASSERT_MSG()
#endif






#endif
