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

#ifndef _XCHIP_PLUGINS_IPLUGIN_H_
#define _XCHIP_PLUGINS_IPLUGIN_H_


#ifdef _WIN32
#define XCHIP_EXPORT _declspec(dllexport)
#elif defined(__linux__) || defined(__APPLE__)
#define XCHIP_EXPORT
#else
#define XCHIP_EXPORT
#error "Unknown Plataform for XCHIP_EXPORT"
#endif

#define XCHIP_LOAD_PLUGIN_SYM "XCHIP_LoadPlugin"
#define XCHIP_FREE_PLUGIN_SYM "XCHIP_FreePlugin"


namespace xchip {



class iPlugin;
using PluginLoader = iPlugin* (*)();
using PluginDeleter = void(*)(const iPlugin*);


class iPlugin
{
public:
	virtual ~iPlugin() = default;
	virtual void Dispose() noexcept = 0;
	virtual bool IsInitialized() const noexcept = 0;
	virtual const char* GetPluginName() const noexcept = 0;
	virtual const char* GetPluginVersion() const noexcept = 0;
	virtual PluginDeleter GetPluginDeleter() const noexcept = 0;
};





}






#endif
