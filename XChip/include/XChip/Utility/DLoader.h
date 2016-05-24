#ifndef _XCHIP_UTILITY_DLOADER_H_
#define _XCHIP_UTILITY_DLOADER_H_

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#error "Unknown Plataform utility DLoader"
#endif

#include <string>

namespace xchip { namespace utility {



class DLoader
{
public:
	DLoader() = default;
	DLoader(DLoader&&) noexcept;
	DLoader(const DLoader&) = delete;
	DLoader& operator=(DLoader&&) noexcept;
	DLoader& operator=(const DLoader&) = delete;
	~DLoader();
	void Free() noexcept;
	bool Load(const std::string& dlPath);
	void* GetSymbol(const std::string& symbol);
	void Swap(DLoader& other) noexcept;
private:
#if defined(__linux__) || defined(__APPLE__)
	void* _handle = nullptr;
#elif defined(_WIN32)
	HMODULE _handle = nullptr;
#endif

};





}}


#endif

