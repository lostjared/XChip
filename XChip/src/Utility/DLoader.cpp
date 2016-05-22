#include <XChip/Utility/DLoader.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Log.h>

namespace xchip { namespace utility {




DLoader::DLoader()
{

}

DLoader::~DLoader()
{
	if(hplugin)
		this->Free();
}
void DLoader::Free()
{

#if defined(__linux__) || defined(__APPLE__)
	dlclose(hplugin);
#elif defined(_WIN32)
	FreeLibrary(hplugin);	
#endif
	hplugin = nullptr;
}


bool DLoader::Load(const std::string& dlPath)
{
	using namespace literals;

	if(hplugin)
		this->Free();

#if defined(__linux__)
	constexpr const char* const postfix = ".so";

#elif defined(__APPLE__)
	constexpr const char* const postfix = ".dylib"
#endif

#if defined(__linux__) || defined(__APPLE__)

	hplugin = dlopen(dlPath.c_str(), RTLD_LAZY);
	if(!hplugin)
	{
		const std::string dlPathFix = dlPath + postfix;
		LOGerr("Error Loading "_s + dlPath);
		LOGerr("Trying with postfix added: "_s + dlPathFix);

		hplugin = dlopen(dlPathFix.c_str(), RTLD_LAZY);

		if(!hplugin)
		{
			const char* error = dlerror();
			LOGerr("Could not load shared library: "_s + error);
			return false;
		}
	}


#elif defined(_WIN32)

#endif


	return true;
}

void* DLoader::GetSymbol(const std::string& symbolName)
{
	using namespace literals; 
	ASSERT_MSG(hplugin != nullptr, "Attempt to Get symbol from null shared library");


#if defined(__linux__) || defined(__APPLE__)

	dlerror(); // clean
	void* symbolAddr = reinterpret_cast<void*>( dlsym(hplugin, symbolName.c_str() ) );
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



