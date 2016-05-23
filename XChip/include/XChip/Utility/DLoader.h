#ifndef _XCHIP_UTILITY_DLOADER_H_
#define _XCHIP_UTILITY_DLOADER_H_

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include <string>

namespace xchip { namespace utility {



class DLoader
{
public:
	DLoader();
	~DLoader();
	void Free();
	bool Load(const std::string& dlPath);
	void* GetSymbol(const std::string& symbol);
private:
#if defined(__linux__) || defined(__APPLE__)
	void* _hplugin = nullptr;
#elif defined(_WIN32)
	HMODULE _hplugin = nullptr;
#endif

};





}}


#endif

