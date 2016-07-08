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
#include "SDL_main.h"
#include <Utix/Log.h>
#include <Utix/Assert.h>
#include <Utix/ScopeExit.h>
#include <XChip/Core/Emulator.h>
#include <XChip/Core/Instructions.h>
#include <XChip/Plugins/UniquePlugin.h>
#include <XChip/Plugins/SDLPlugins/SdlRender.h>
#include <XChip/Plugins/SDLPlugins/SdlAndroidInput.h>
#include <XChip/Plugins/SDLPlugins/SdlSound.h>




static bool InitializeEmulator(xchip::Emulator* const emulator);


int main(int, char**)
{
	ASSERT_MSG(false, "TESTING ASSERT MSG");


	xchip::Emulator* const emulator = new(std::nothrow) xchip::Emulator();
	
	if(emulator == nullptr)
		return EXIT_FAILURE;

	const auto emulator_cleanup = utix::MakeScopeExit([emulator]() noexcept {
		delete emulator;
	});


	if(!InitializeEmulator(emulator))
		return EXIT_FAILURE;

	else if(!emulator->LoadRom("/data/local/tmp/Game.ch8"))
		return EXIT_FAILURE;

	
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



static bool InitializeEmulator(xchip::Emulator* const emulator)
{
	using utix::move;

	xchip::UniqueRender render;
	xchip::UniqueInput input;
	xchip::UniqueSound sound;

	render.Load(new(std::nothrow) xchip::SdlRender());
	input.Load(new(std::nothrow) xchip::SdlAndroidInput());
	sound.Load(new(std::nothrow) xchip::SdlSound());

	if ( emulator->Initialize(move(render), move(input), move(sound)) ) 
	{
		auto android_input = static_cast<xchip::SdlAndroidInput*>(emulator->GetInput());
		auto render = static_cast<xchip::SdlRender*>(emulator->GetRender());
		render->SetFullScreen(true);
		android_input->SetMiddleScreen(render->GetWindowSize().x / 2);
		return true;
	}

	return false;
}







