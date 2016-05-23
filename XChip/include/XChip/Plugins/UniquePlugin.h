#ifndef _XCHIP_PLUGINS_UNIQUEPLUGIN_H_
#define _XCHIP_PLUGINS_UNIQUEPLUGIN_H_
#include "iPlugin.h"
#include <XChip/Utility/DLoader.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/BaseTraits.h>
#include <XChip/Utility/Memory.h>

namespace xchip {



template<class T>
class UniquePlugin
{
	static_assert(utility::is_same<T, iRender>::value || 
		           utility::is_same<T, iInput>::value || 
		           utility::is_same<T, iSound>::value, 
		           "UniquePlugin must be a type of iPlugin interface");
public:
	UniquePlugin();
	UniquePlugin(std::nullptr_t);
	UniquePlugin(UniquePlugin&& rhs);
	UniquePlugin& operator=(UniquePlugin&& rhs);
	~UniquePlugin();
	const T* get() const;
	T* get();
	bool Load(const std::string& dlPath);
	void Free();
	void Swap(UniquePlugin& rhs);

private:
	utility::DLoader _dloader;
	std::unique_ptr<T> _plugin;

};







template<class T>
UniquePlugin<T>::UniquePlugin()
	: _plugin(nullptr)
{
}



template<class T>
UniquePlugin<T>::UniquePlugin(std::nullptr_t)
	: _plugin(nullptr)
{
}



template<class T>
UniquePlugin<T>::UniquePlugin(UniquePlugin&& rhs)
	: _plugin(std::move(rhs._plugin)),
	_dloader(std::move(rhs._dloader))
{
}



template<class T>
UniquePlugin<T>& UniquePlugin<T>::operator=(UniquePlugin&& rhs)
{
	_plugin = std::move(rhs._plugin);
	_dloader = std::move(rhs._dloader);
	return *this;
}



template<class T>
UniquePlugin<T>::~UniquePlugin()
{
	if (_plugin)
		this->Free();
}



template<class T>
const T* UniquePlugin<T>::get() const
{
	return _plugin.get();
}


template<class T>
T* UniquePlugin<T>::get()
{
	return _plugin.get();
}




template<class T>
bool UniquePlugin<T>::Load(const std::string& dlPath)
{
	using namespace utility::literals;

	if (_plugin != nullptr)
		this->Free();

	const auto cleanup = utility::make_scope_exit([this]()noexcept
	{
		if (this->_plugin == nullptr)
			this->Free();
	});


	if (!_dloader.Load(dlPath))
		return false;

	const auto pluginLoader = reinterpret_cast<PluginLoader> (_dloader.GetSymbol(XCHIP_LOAD_PLUGIN_SYM));

	if (!pluginLoader)
	{
		utility::LOGerr("Failed to get the plugin loader symbol!");
		return false;
	}

	_plugin.reset(static_cast<T*>(pluginLoader()));

	if (_plugin == nullptr)
	{
		utility::LOGerr("PluginLoader function in "_s + dlPath + ", returned a null plugin.");
		return false;
	}

	return true;
}



template<class T>
void UniquePlugin<T>::Free()
{
	const auto rawPlugin = _plugin.release();
	rawPlugin->GetPluginDeleter()(rawPlugin);
	_dloader.Free();
}





template<class T>
void UniquePlugin<T>::Swap(UniquePlugin& rhs)
{
	auto auxPlugin = std::move(this->_plugin);
	auto auxDLoader = std::move(this->_dloader);
	this->_plugin = std::move(rhs._plugin);
	this->_dloader = std::move(rhs._dloader);
	rhs._plugin = std::move(auxPlugin);
	rhs._dloader = std::move(auxDLoader);
}






















}

#endif
