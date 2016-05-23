#include <XChip/Utility/DLoader.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Log.h>

namespace xchip { namespace utility {


DLoader::DLoader()
{

}

DLoader::~DLoader()
{
	if(_hplugin)
		this->Free();
}
void DLoader::Free()
{

#if defined(__linux__) || defined(__APPLE__)
	dlclose(_hplugin);
#elif defined(_WIN32)
	FreeLibrary(_hplugin);	
#endif
	_hplugin = nullptr;
}


bool DLoader::Load(const std::string& dlPath)
{
	using namespace literals;

	if(_hplugin)
		this->Free();

#if defined(__linux__)
	constexpr const char* const postfix = ".so";
#elif defined(__APPLE__)
	constexpr const char* const postfix = ".dylib";
#elif defined(_WIN32)
	constexpr const char* const postfix = ".dll";
#endif


#if defined(__linux__) || defined(__APPLE__)

	_hplugin = dlopen(dlPath.c_str(), RTLD_LAZY);
	if(!_hplugin)
	{
		const std::string dlPathFix = dlPath + postfix;
		LOGerr("Error Loading "_s + dlPath);
		LOGerr("Trying with postfix added: "_s + dlPathFix);

		_hplugin = dlopen(dlPathFix.c_str(), RTLD_LAZY);

		if(!_hplugin)
		{
			const char* error = dlerror();
			LOGerr("Could not load shared library: "_s + error);
			return false;
		}
	}


#elif defined(_WIN32)

	_hplugin = LoadLibrary(dlPath.c_str());

	if (!_hplugin)
	{
		const std::string dlPathFix = dlPath + postfix;
		LOGerr("Error Loading "_s + dlPath);
		LOGerr("Trying with postfix added: "_s + dlPathFix);
		_hplugin = LoadLibrary(dlPath.c_str());
		
		if (!_hplugin)
		{
			const int errorCode = GetLastError();
			LOGerr("Could not load "_s + dlPath);
			LOGerr("Error Code: "_s + std::to_string(errorCode));
			return false;
		}
	}


#endif


	return true;
}

void* DLoader::GetSymbol(const std::string& symbolName)
{
	using namespace literals; 
	ASSERT_MSG(_hplugin != nullptr, "Attempt to Get symbol from null shared library");


#if defined(__linux__) || defined(__APPLE__)

	dlerror(); // clean
	void* symbolAddr = reinterpret_cast<void*>( dlsym(_hplugin, symbolName.c_str() ) );
	const char* error = dlerror();
	if(error)
	{
		LOGerr("Failed to get symbol addr: "_s + error);
		return nullptr;
	}
	
#elif defined(_WIN32)
	GetLastError(); // clean
	void* symbolAddr = GetProcAddress(_hplugin, symbolName.c_str());
	const auto errorCode = GetLastError();
	if (!symbolAddr && errorCode)
	{
		LOGerr("Failed to get symbol addr: Error Code: "_s + std::to_string(errorCode));
		return nullptr;
	}

#endif	


	return symbolAddr;
}


















}}



