#ifndef ASSERT_H
#define ASSERT_H
#include <cassert>

#define dynamic_assert(condition, msg){ if(!condition){ std::cerr << msg << std::endl; assert(condition); } }




#endif // ASSERT_H

