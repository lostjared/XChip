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

#include <utility>

#include <SDL2/SDL.h>
#include <XChip/Media/iMediaPlugin.h>
#include <XChip/Media/SDLMedia/SdlSystem.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Alloc.h>
 

namespace xchip {

// subsystem flag and name
using FlagAndName = std::pair<Uint32, const char*>;
FlagAndName get_sdl_subsystem(const SdlSystem::System sys) noexcept;
template<class T, const size_t sz>
bool _all_of(const T(&arr)[sz], const T val);
constexpr size_t toSizeT(SdlSystem::System sys) noexcept;





SDL_Event g_sdlEvent;
int SdlSystem::s_nSystems[3] = { 0, 0, 0 };
bool SdlSystem::s_SubSystems[3] = { false, false, false };




SdlSystem::SdlSystem(const System sys)
	: _sys(sys)
{
	using utility::arr_size;
	ASSERT_MSG(toSizeT(sys) < arr_size(s_nSystems)
		&& toSizeT(sys) < arr_size(s_SubSystems),
		"sys greater than systems/subsystems array counter");

	++s_nSystems[toSizeT(_sys)];
}



SdlSystem::~SdlSystem()
{
	using namespace utility;
	using namespace utility::literals;

	--s_nSystems[toSizeT(_sys)];
	
	// if this is the last instance of this System (_sys)
	// and Sdl Subsystem of (_sys) is On, then Close it
	if (!s_nSystems[toSizeT(_sys)]
		&& s_SubSystems[toSizeT(_sys)])
	{
		const auto flagAndName = get_sdl_subsystem(_sys);
		LOG("Closing "_s + flagAndName.second);

		SDL_QuitSubSystem(flagAndName.first);
		s_SubSystems[toSizeT(_sys)] = false;
	}
	
	if ( _all_of(s_SubSystems, false) )
	{
		LOG("Quitting SDL...");
		SDL_Quit();
	}
}



bool SdlSystem::InitSubSystem()
{
	using namespace utility;
	using namespace utility::literals;


	// if the SDL Subsystem of (_sys) is off
	// then initialize it
	if( !s_SubSystems[toSizeT(_sys)] )
	{
		const auto flagAndName = get_sdl_subsystem(_sys);
		LOG("Initializing "_s + flagAndName.second);

		if (SDL_InitSubSystem(flagAndName.first) != SDL_FALSE)
		{
			LOGerr("Could not initialize "_s + flagAndName.second + " " + SDL_GetError());
			return false;
		}

		s_SubSystems[toSizeT(_sys)] = true;
	}
	
	return true;
}


void SdlSystem::PollEvent()
{
	using namespace utility;
	using namespace utility::literals;
	static Timer eventTimer (450_hz);
	if (eventTimer.Finished()) {
		SDL_PollEvent(&g_sdlEvent);
		eventTimer.Start();
	}
}








FlagAndName get_sdl_subsystem(const SdlSystem::System sys) noexcept
{
	using System = SdlSystem::System;

	switch(sys)
	{
		case System::Render: 
			return FlagAndName{ SDL_INIT_VIDEO, "SDL Video Subsystem..." };
		case System::Input:
			return FlagAndName{ SDL_INIT_EVENTS, "SDL Input Subsystem..." };
		case System::Sound:
		 	return FlagAndName{ SDL_INIT_AUDIO, "SDL Audio Subsystem..." };
		default:	
	 		return FlagAndName{ 0, "Unknown SDL Subsystem!" };
	}
}



constexpr size_t toSizeT(SdlSystem::System sys) noexcept {
	return static_cast<size_t>(sys);
}


template<class T, const size_t sz>
bool _all_of(const T(&arr)[sz], const T val)
{
	for (const auto it : arr) 
		if (it != val) 
			return false;

	return true;
}









}


