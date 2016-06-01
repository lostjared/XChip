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

#include <csignal>
#include <iostream>
#include <dlfcn.h>
#include <XChip/Plugins/iPlugin.h>
#include <XChip/Utility/Log.h>

int main(int argc, char **argv)
{
	using LoadPtr = xchip::iPlugin* (*)();
	using FreePtr = void (*)(xchip::iPlugin*);
	volatile auto x=reinterpret_cast<void volatile * volatile >(&xchip::utility::LogError);
	volatile auto y=reinterpret_cast<void volatile * volatile >(&xchip::utility::Log);
	auto handle = dlopen("./XChipSFMLInput.so", RTLD_LAZY);

	auto loadFun = (LoadPtr) dlsym(handle, "XCHIP_LoadPlugin");
	
	auto plugin = loadFun();

	std::cout << plugin->GetPluginName() << std::endl;
	std::cout << plugin->GetPluginVersion() << std::endl;

	auto freeFun = (FreePtr) dlsym(handle, "XCHIP_FreePlugin");

	freeFun(plugin);

	dlclose(handle);

	return EXIT_SUCCESS;
}












