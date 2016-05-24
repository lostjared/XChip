/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/



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
	UniquePlugin(UniquePlugin&& rhs) noexcept;
	UniquePlugin(const UniquePlugin& rhs) = delete;
	UniquePlugin& operator=(UniquePlugin&& rhs) noexcept;
	UniquePlugin& operator=(const UniquePlugin& rhs) = delete;
	~UniquePlugin();
	bool Load(const std::string& dlPath);
	void Free();
	const T* get() const;
	T* get();
	void Swap(UniquePlugin& rhs) noexcept;



private:
	static void call_deleter(utility::DLoader&, iPlugin*) noexcept;
	utility::DLoader _dloader;
	T* _plugin = nullptr;
};



template<class T>
UniquePlugin<T>::UniquePlugin(std::nullptr_t)
{
}



template<class T>
UniquePlugin<T>::UniquePlugin(UniquePlugin&& rhs) noexcept
	:  _dloader(std::move(rhs._dloader)),
	_plugin(rhs._plugin)
{
	rhs._plugin = nullptr;
}



template<class T>
UniquePlugin<T>& UniquePlugin<T>::operator=(UniquePlugin&& rhs) noexcept
{
	this->Swap(rhs);
	return *this;
}



template<class T>
UniquePlugin<T>::~UniquePlugin()
{
	this->Free();
}



template<class T>
bool UniquePlugin<T>::Load(const std::string& dlPath)
{
	using namespace utility::literals;
	using utility::DLoader;

	DLoader newLoader;

	if (!newLoader.Load(dlPath))
		return false;


	const auto pluginLoader = reinterpret_cast<PluginLoader>( newLoader.GetSymbol(XCHIP_LOAD_PLUGIN_SYM) );

	if (!pluginLoader)
	{
		utility::LOGerr("Failed to get the plugin loader symbol!");
		return false;
	}


	auto iplug = static_cast<iPlugin*>(pluginLoader());

	if (!iplug)
	{
		utility::LOGerr("PluginLoader function in "_s + dlPath + ", returned a null plugin.");
		return false;
	}


	T* newPlugin = dynamic_cast<T*>(iplug);

	if(!newPlugin)
	{
		utility::LOGerr("Error Loading plugin: "_s + dlPath);
		utility::LOGerr("failed to dynamic cast from iPlugin.");
		call_deleter(_dloader, iplug);
		return false;
	}


	if(_plugin)
		call_deleter(_dloader, _plugin);

	_dloader = std::move(newLoader);
	_plugin = newPlugin;
	return true;
}





template<class T>
void UniquePlugin<T>::Free()
{
	if(_plugin) 
	{
		call_deleter(_dloader, _plugin);
		_plugin = nullptr;
		_dloader.Free();
	}
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
void UniquePlugin<T>::Swap(UniquePlugin& other) noexcept
{
	this->_dloader.Swap(other._dloader);
	auto* const aux = this->_plugin;
	this->_plugin = other._plugin;
	other._plugin = aux;
}




template<class T>
void UniquePlugin<T>::call_deleter(utility::DLoader& dloader, iPlugin* plugin) noexcept
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
