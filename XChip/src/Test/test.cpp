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

#include <XChip/Core/Emulator.h> 
#include <XChip/Plugins.h>
#include <XChip/Utility.h>



static xchip::utility::DLoader dlrender;
static xchip::utility::DLoader dlinput;
static xchip::utility::DLoader dlsound;
static xchip::Emulator g_emulator;


int main(int argc, char **argv)
{
	using xchip::Emulator;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::PluginLoader;


	if(argc < 2)
	{
		xchip::utility::LOGerr("No game to load.");
		return EXIT_FAILURE;
	}


	try 
	{
		UniqueRender render;
		UniqueInput input;
		UniqueSound sound;

		if(!dlrender.Load("./libXChipSDLRenderPlugin") ||
		   	!dlinput.Load("./libXChipSDLInputPlugin") ||
			!dlsound.Load("./libXChipSDLSoundPlugin") )
		{
			throw std::runtime_error("could not load all plugins");
		}


		const auto loadRender = reinterpret_cast<PluginLoader>( dlrender.GetSymbol("XCHIP_LoadPlugin") );
		const auto loadInput = reinterpret_cast<PluginLoader>( dlinput.GetSymbol("XCHIP_LoadPlugin") );
		const auto loadSound = reinterpret_cast<PluginLoader>( dlsound.GetSymbol("XCHIP_LoadPlugin") );

		if(!loadRender || !loadInput || !loadSound )
			throw std::runtime_error("Could not get plugin Load function");

		render.reset( static_cast<xchip::iRender*>(loadRender()) );
		input.reset( static_cast<xchip::iInput*>(loadInput()) );
		sound.reset( static_cast<xchip::iSound*>( loadSound()) );

		if(!render || !input || !sound)
			throw std::runtime_error("Some LoadPlugin function returned nullptr");

		if (!g_emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
			throw std::runtime_error("Failed to initialize emulator");

		if(!g_emulator.LoadRom(argv[1]))
			throw std::runtime_error("Failed to load rom");
	}
	catch (std::exception& e) 
	{
		std::cout << "Failed to setup emulator: " <<  e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	while (!g_emulator.GetExitFlag())
	{
		g_emulator.UpdateSystems(); 
		g_emulator.HaltForNextFlag();

		if(g_emulator.GetInstrFlag())
			g_emulator.ExecuteInstr();

		if(g_emulator.GetDrawFlag())
			g_emulator.Draw();
	}



	return EXIT_SUCCESS;
}














