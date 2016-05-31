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
#include <XChip/Utility/StdintDef.h>
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
	UniquePlugin(const UniquePlugin& rhs) = delete;
	UniquePlugin& operator=(const UniquePlugin& rhs) = delete;
	UniquePlugin() = default;
	UniquePlugin(const nullptr_t);
	UniquePlugin(UniquePlugin&& rhs) noexcept;
	UniquePlugin& operator=(UniquePlugin&& rhs) noexcept;
	~UniquePlugin();
	bool operator!=(const T* addr) const;
	bool operator==(const T* addr) const;
	bool operator!=(const UniquePlugin& other) const;
	bool operator==(const UniquePlugin& other) const;
	operator bool() const;
	const T* operator->() const;
	const T* get() const;
	T* get();
	T* operator->();
	bool Load(const std::string& dlPath);
	void Free();
	void Swap(UniquePlugin& rhs) noexcept;


private:
	static void call_deleter(utility::DLoader&, iPlugin*) noexcept;
	utility::DLoader _dloader;
	T* _plugin = nullptr;
};



template<class T>
inline UniquePlugin<T>::UniquePlugin(const nullptr_t)
{
}

template<class T>
inline UniquePlugin<T>::UniquePlugin(UniquePlugin&& rhs) noexcept
	:  _dloader(std::move(rhs._dloader)),
	_plugin(rhs._plugin)
{
	rhs._plugin = nullptr;
}



template<class T>
inline UniquePlugin<T>& UniquePlugin<T>::operator=(UniquePlugin&& rhs) noexcept
{
	this->Swap(rhs);
	return *this;
}



template<class T>
inline UniquePlugin<T>::~UniquePlugin()
{
	this->Free();
}





template<class T>
inline bool UniquePlugin<T>::operator!=(const T* addr) const
{
	return _plugin != addr;
}


template<class T>
inline bool UniquePlugin<T>::operator==(const T* addr) const
{
	return _plugin == addr;
}


template<class T>
inline bool UniquePlugin<T>::operator!=(const UniquePlugin& other) const
{
	return this->_plugin != other._plugin;
}


template<class T>
inline bool UniquePlugin<T>::operator==(const UniquePlugin& other) const
{
	return this->_plugin == other._plugin;
}




template<class T>
inline UniquePlugin<T>::operator bool() const
{
	return _plugin != nullptr;
}




template<class T>
inline const T* UniquePlugin<T>::operator->() const
{
	return _plugin;
}


template<class T>
inline const T* UniquePlugin<T>::get() const
{
	return _plugin;
}



template<class T>
inline T* UniquePlugin<T>::operator->()
{
	return _plugin;
}


template<class T>
inline T* UniquePlugin<T>::get()
{
	return _plugin;
}



template<class T>
bool UniquePlugin<T>::Load(const std::string& dlPath)
{
	using namespace utility;

	DLoader newLoader;

	if (!newLoader.Load(dlPath))
		return false;


	const auto pluginLoader = reinterpret_cast<PluginLoader>( newLoader.GetSymbol(XCHIP_LOAD_PLUGIN_SYM) );

	if (!pluginLoader)
	{
		LogError("Failed to get the plugin loader symbol!");
		return false;
	}


	auto iplug = static_cast<iPlugin*>(pluginLoader());

	if (!iplug)
	{
		LogError("PluginLoader function in %s, returned a null plugin!", dlPath.c_str());
		return false;
	}


	T* newPlugin = dynamic_cast<T*>(iplug);

	if(!newPlugin)
	{
		LogError("Error Loading plugin: %s. Failed to dynamic cast from iPlugin!", dlPath.c_str());
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
void UniquePlugin<T>::Swap(UniquePlugin& other) noexcept
{
	if(&other != this)
	{
		this->_dloader.Swap(other._dloader);
		auto* const aux = this->_plugin;
		this->_plugin = other._plugin;
		other._plugin = aux;
	}
}









template<class T>
void UniquePlugin<T>::call_deleter(utility::DLoader& dloader, iPlugin* plugin) noexcept
{
	using namespace utility;

	auto pluginDeleter = reinterpret_cast<PluginDeleter>( dloader.GetSymbol(XCHIP_FREE_PLUGIN_SYM) );

	if(pluginDeleter)
		pluginDeleter(plugin);
	else
	{
		LogError("Failed to load plugin deleter. trying GetPluginDeleter method...");
		pluginDeleter = plugin->GetPluginDeleter();

		if(pluginDeleter)
			pluginDeleter(plugin);
		else 
		{
			LogError("GetPluginDeleter failed...  Trying deleting in place, prepare for crash...");
			delete plugin;
		}
	
	}
}


















}

#endif
