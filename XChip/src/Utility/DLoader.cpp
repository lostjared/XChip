#include <XChip/Utility/DLoader.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Log.h>

namespace xchip { namespace utility {


DLoader::DLoader(DLoader&& rhs) noexcept
	: _handle(rhs._handle)
{
	rhs._handle = nullptr;
}


DLoader::~DLoader()
{
	this->Free();
}


DLoader& DLoader::operator=(DLoader&& rhs) noexcept
{
	this->Swap(rhs);
	return *this;
}





bool DLoader::Load(const std::string& dlPath)
{
	using namespace literals;

#if defined(__linux__)
	constexpr const char* const postfix = ".so";
#elif defined(__APPLE__)
	constexpr const char* const postfix = ".dylib";
#elif defined(_WIN32)
	constexpr const char* const postfix = ".dll";
#endif


#if defined(__linux__) || defined(__APPLE__)

	auto newHandle = dlopen(dlPath.c_str(), RTLD_LAZY);
	
	if (!newHandle)
	{
		const std::string dlPathFix = dlPath + postfix;
		newHandle = dlopen(dlPathFix.c_str(), RTLD_LAZY);

		if (!newHandle)
		{
			const char* error = dlerror();
			LOGerr("Could not load shared library: "_s + error);
			return false;
		}
	}


#elif defined(_WIN32)

	auto newHandle = LoadLibrary(dlPath.c_str());

	if(!newHandle)
	{
		const std::string dlPathFix = dlPath + postfix;
		newHandle = LoadLibrary(dlPath.c_str());
		if (!newHandle)
		{
			const int errorCode = GetLastError();
			LOGerr("Could not load "_s + dlPath + ", or " + dlPathFix + " ...");
			LOGerr("Error Code: "_s + std::to_string(errorCode));
			return false;
		}
	}


#endif

	this->Free();
	_handle = newHandle;
	return true;
}





void DLoader::Free() noexcept
{
	if(_handle)
	{

#if defined(__linux__) || defined(__APPLE__)
		dlclose(_handle);
#elif defined(_WIN32)
		FreeLibrary(_handle);	
#endif
	

		_handle = nullptr;
	}
}




void* DLoader::GetSymbol(const std::string& symbolName)
{
	using namespace literals; 
	ASSERT_MSG(_handle != nullptr, "Attempt to Get symbol from null shared library");


#if defined(__linux__) || defined(__APPLE__)

	dlerror(); // clean
	void* symbolAddr = reinterpret_cast<void*>( dlsym(_handle, symbolName.c_str() ) );
	const char* error = dlerror();
	if(error)
	{
		LOGerr("Failed to get symbol addr: "_s + error);
		return nullptr;
	}
	
#elif defined(_WIN32)
	SetLastError(0); // clean
	void* symbolAddr = GetProcAddress(_handle, symbolName.c_str());
	const auto errorCode = GetLastError();
	if (!symbolAddr && errorCode)
	{
		LOGerr("Failed to get symbol addr: Error Code: "_s + std::to_string(errorCode));
		return nullptr;
	}

#endif	


	return symbolAddr;
}





void DLoader::Swap(DLoader& other) noexcept
{
	auto* const aux = this->_handle;
	this->_handle = other._handle;
	other._handle = aux;
}












}}



