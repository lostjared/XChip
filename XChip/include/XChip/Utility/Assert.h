#ifndef _XCHIP_UTILITY_ASSERT_H_
#define _XCHIP_UTILITY_ASSERT_H_
// #define NDEBUG
#include <cassert>
#include "Log.h"

#ifndef NDEBUG
#define ASSERT_MSG(cond, msg) { if(!(cond)) {xchip::utility::LOGerr(msg); assert(cond); } }
#else
#define ASSERT_MSG()
#endif






#endif
