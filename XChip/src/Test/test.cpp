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

	Emulator emulator;
	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;


	if (!(render.Load("./XChipSDLRender") &&
		input.Load("./XChipSDLInput") &&
		sound.Load("./XChipSDLSound")))
	{
		xchip::utility::LOGerr("Could not load plugins");
		return EXIT_FAILURE;
	}

	if (!emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
		return EXIT_FAILURE;
	else if (!emulator.LoadRom(argv[1]))
		return EXIT_FAILURE;
	

	while (!emulator.GetExitFlag())
	{
		emulator.UpdateSystems();
		emulator.HaltForNextFlag();
		if (emulator.GetInstrFlag())
			emulator.ExecuteInstr();
		if (emulator.GetDrawFlag())
			emulator.Draw();
	}




	return EXIT_SUCCESS;
}












