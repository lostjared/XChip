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

// TODO: add support for plugins as shared libraries
// for that is needed to compile SDL plugins as .so own their own
// and pack it into the .apk somehow

#include "SDL_main.h"
#include <android/log.h>
#include <Utix/Log.h>
#include <Utix/ScopeExit.h>
#include <XChip/Core/Emulator.h>
#include <XChip/Core/Instructions.h>
#include <XChip/Plugins/UniquePlugin.h>
#include <XChip/Plugins/SDLPlugins/SdlRender.h>
#include <XChip/Plugins/SDLPlugins/SdlInput.h>
#include <XChip/Plugins/SDLPlugins/SdlSound.h>



int main(int, char**)
{

	utix::Log("Testing Utix Log");
	using utix::move;
	using utix::MakeScopeExit;
	using xchip::Emulator;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;


	Emulator* emulator = new(std::nothrow) Emulator();
	if(emulator == nullptr)
		return EXIT_FAILURE;

	const auto emulator_cleanup = MakeScopeExit([emulator]()noexcept{
		delete emulator;
	});

	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;
	render.Load(new(std::nothrow) xchip::SdlRender());
	input.Load(new(std::nothrow) xchip::SdlInput());
	sound.Load(new(std::nothrow) xchip::SdlSound());

	if ( ! emulator->Initialize(move(render), move(input), move(sound)) ) {
		return EXIT_FAILURE;
	}

	else if( ! emulator->LoadRom("/data/local/tmp/Game.ch8") ) {
		return EXIT_FAILURE;
	}

	emulator->GetRender()->SetFullScreen(true);

	while(emulator->GetExitFlag() == false) 
	{
		emulator->UpdateSystems();
		emulator->HaltForNextFlag();
		if( emulator->GetInstrFlag() )
			emulator->ExecuteInstr();
		if( emulator->GetDrawFlag() )
			emulator->Draw();
	}

	return EXIT_SUCCESS;
}

