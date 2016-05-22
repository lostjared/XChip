#ifndef _XCHIP_UTILITY_PLUGIN_H_
#define _XCHIP_UTILITY_PLUGIN_H_

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <windows.h>
#endif



namespace xchip { namespace utility {



class Plugin
{
public:
	Plugin();
	~Plugin();
	void Free();
	bool Load(const char* dlPath);
	void* GetAddr(const char* symbol);
private:
#if defined(__linux__) || defined(__APPLE__)
	void* hplugin = nullptr;
#elif defined(_WIN32)

#endif

};





}}


#endif

