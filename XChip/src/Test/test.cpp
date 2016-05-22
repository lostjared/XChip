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
#include <XChip/Media/Interfaces.h>
#include <XChip/Utility/Memory.h>
#include <XChip/Utility/Plugin.h>
#include <XChip/Utility/Log.h>



static xchip::utility::Plugin prender;
static xchip::utility::Plugin pinput;
static xchip::utility::Plugin psound;
static xchip::Emulator g_emulator;


int main(int argc, char **argv)
{
	using xchip::Emulator;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;



	using LoadPluginFunc = xchip::iMediaPlugin* (*)();

	if(argc < 2)
	{
		xchip::utility::LOGerr("No game to load.");
		return EXIT_FAILURE;
	}


	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;

	try {
		if(!prender.Load("./libXChipSDLRenderPlugin.so") ||
		   	!pinput.Load("./libXChipSDLInputPlugin.so") ||
			!psound.Load("./libXChipSDLSoundPlugin.so") )
		{
			throw std::runtime_error("could not load all plugins");
		}


		LoadPluginFunc loadRender = reinterpret_cast<LoadPluginFunc>( prender.GetAddr("XCHIP_LoadPlugin") );
		LoadPluginFunc loadInput = reinterpret_cast<LoadPluginFunc>( pinput.GetAddr("XCHIP_LoadPlugin") );
		LoadPluginFunc loadSound = reinterpret_cast<LoadPluginFunc>( psound.GetAddr("XCHIP_LoadPlugin") );

		if(!loadRender || !loadInput || !loadSound )
			throw std::runtime_error("Could not get plugin Load function");

		render.reset( reinterpret_cast<xchip::iRender*>( loadRender() ) );
		input.reset( reinterpret_cast<xchip::iInput*>( loadInput()) );
		sound.reset( reinterpret_cast<xchip::iSound*>( loadSound()) );
	}
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	
	if (!g_emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
		return EXIT_FAILURE;



	if(!g_emulator.LoadRom(argv[1]))
		return EXIT_FAILURE;
	
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














