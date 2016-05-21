#include <XChip/Utility/Plugin.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Log.h>

namespace xchip { namespace utility {




Plugin::Plugin()
{

}

Plugin::~Plugin()
{
	if(hplugin)
		this->Free();
}
void Plugin::Free()
{

#if defined(__linux__) || defined(__APPLE__)
	dlclose(hplugin);
#elif defined(_WIN32)
	FreeLibrary(hplugin);	
#endif
	hplugin = nullptr;
}


bool Plugin::Load(const char* dlPath)
{
	using namespace literals;
	if(hplugin)
		this->Free();


#if defined(__linux__) || defined(__APPLE__)

	hplugin = dlopen(dlPath, RTLD_LAZY);
	if(!hplugin)
	{
		const char* error = dlerror();
		LOGerr("Could not load shared library: "_s + error);
		return false;
	}

	return true;
	
#elif defined(_WIN32)

#endif



}

void* Plugin::GetAddr(const char* symbolName)
{
	ASSERT_MSG(hplugin != nullptr, "Attempt to Get symbol from null shared library");

	using namespace literals; 
	

	#if defined(__linux__) || defined(__APPLE__)
	
	dlerror(); // clean
	void* symbolAddr = reinterpret_cast<void*>( dlsym(hplugin, symbolName) );
	const char* error = dlerror();
	if(error)
	{
		LOGerr("Failed to get symbol addr: "_s + error);
		return nullptr;
	}


	#elif defined(_WIN32)
	#endif	


	return symbolAddr;
}


















}}



