#ifndef _XCHIP_PLUGINS_UNIQUEPLUGIN_H_
#define _XCHIP_PLUGINS_UNIQUEPLUGIN_H_

#include "iPlugin.h"
#include <XChip/Utility/DLoader.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/BaseTraits.h>

namespace xchip {



template<class T>
class UniquePlugin
{
	static_assert(utility::is_same<T, iRender>::value || 
		           utility::is_same<T, iInput>::value || 
		           utility::is_same<T, iSound>::value, 
		           "UniquePlugin must be a type of iPlugin interface");
public:
	UniquePlugin() = default;
	UniquePlugin(std::nullptr_t);
	UniquePlugin(UniquePlugin&& rhs);
	UniquePlugin(const UniquePlugin& rhs) = delete;
	UniquePlugin& operator=(UniquePlugin&& rhs);
	UniquePlugin& operator=(const UniquePlugin& rhs) = delete;
	~UniquePlugin();
	bool Load(const std::string& dlPath);
	void Free();
	const T* get() const;
	T* get();
	void Swap(UniquePlugin& rhs);
private:
	static void call_deleter(utility::DLoader&, iPlugin*);
	utility::DLoader _dloader;
	T* _plugin = nullptr;
};





template<class T>
UniquePlugin<T>::UniquePlugin(std::nullptr_t)
	: _plugin(nullptr)
{
}



template<class T>
UniquePlugin<T>::UniquePlugin(UniquePlugin&& rhs)
	:  _dloader(std::move(rhs._dloader)),
	_plugin(rhs._plugin)
{
	rhs._plugin = nullptr;
}



template<class T>
UniquePlugin<T>& UniquePlugin<T>::operator=(UniquePlugin&& rhs)
{
	if(_plugin)
		this->Free();

	_dloader = std::move(rhs._dloader);
	_plugin = rhs._plugin;
	rhs._plugin = nullptr;
	return *this;
}



template<class T>
UniquePlugin<T>::~UniquePlugin()
{
	if (_plugin)
		this->Free();
}



template<class T>
bool UniquePlugin<T>::Load(const std::string& dlPath)
{
	using namespace utility::literals;

	if (_plugin)
		this->Free();

	if (!_dloader.Load(dlPath))
		return false;

	const auto cleanup = utility::make_scope_exit([this]() noexcept
	{
		if (!_plugin)
			_dloader.Free();
	});


	const auto pluginLoader = reinterpret_cast<PluginLoader> (_dloader.GetSymbol(XCHIP_LOAD_PLUGIN_SYM));

	if (!pluginLoader)
	{
		utility::LOGerr("Failed to get the plugin loader symbol!");
		return false;
	}


	auto* iplug = static_cast<iPlugin*>(pluginLoader());

	if (!iplug)
	{
		utility::LOGerr("PluginLoader function in "_s + dlPath + ", returned a null plugin.");
		return false;
	}

	_plugin = dynamic_cast<T*>(iplug);

	if(!_plugin)
	{
		utility::LOGerr("Error Loading plugin: "_s + dlPath);
		utility::LOGerr("failed to dynamic cast from iPlugin.");
		call_deleter(_dloader, iplug);
		return false;
	}

	return true;
}





template<class T>
void UniquePlugin<T>::Free()
{
	if(_plugin) 
	{
		call_deleter(_dloader, static_cast<iPlugin*>(_plugin));
		_plugin = nullptr;
	}

	_dloader.Free();
}






template<class T>
const T* UniquePlugin<T>::get() const
{
	return _plugin;
}


template<class T>
T* UniquePlugin<T>::get()
{
	return _plugin;
}





template<class T>
void UniquePlugin<T>::Swap(UniquePlugin& rhs)
{
	auto auxPlugin = this->_plugin;
	auto auxDLoader = std::move(this->_dloader);
	this->_plugin = rhs._plugin;
	this->_dloader = std::move(rhs._dloader);
	rhs._plugin = auxPlugin;
	rhs._dloader = std::move(auxDLoader);
}




template<class T>
void UniquePlugin<T>::call_deleter(utility::DLoader& dloader, iPlugin* plugin)
{
	auto pluginDeleter = reinterpret_cast<PluginDeleter>( dloader.GetSymbol(XCHIP_FREE_PLUGIN_SYM) );

	if(pluginDeleter)
		pluginDeleter(plugin);
	else
	{
		utility::LOGerr("Failed to load plugin deleter. trying GetPluginDeleter method...");
		pluginDeleter = plugin->GetPluginDeleter();

		if(pluginDeleter)
			pluginDeleter(plugin);
		else 
		{
			utility::LOGerr("GetPluginDeleter failed...  Trying deleting in place, prepare for crash...");
			delete plugin;
		}
	
	}
}




}

#endif
