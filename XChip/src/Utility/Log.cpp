
#include <cstdio>
#include <stdarg.h>
#include <XChip/Utility/Log.h>







namespace xchip { namespace utility {




void LOG(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);
	std::vfprintf(stdout, fmtString, args);
	va_end(args);
}


void LOGerr(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);
	std::vfprintf(stderr, fmtString, args);
	va_end(args);
}









}}
