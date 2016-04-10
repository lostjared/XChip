#include <utility>

#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlSystem.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Assert.h>
#include <XChip/Utility/Alloc.h>
 

namespace xchip {

// subsystem flag and name
using FlagAndName = std::pair<Uint32, const char*>;
constexpr FlagAndName get_sdl_subsystem(const SdlSystem::System sys) noexcept;
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
		"SdlSystem::SdlSystem: sys greater than systems/subsystems array counter");

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







constexpr FlagAndName get_sdl_subsystem(const SdlSystem::System sys) noexcept
{
	using System = SdlSystem::System;

	return (sys == System::Render)
		? FlagAndName{ SDL_INIT_VIDEO, "SDL Video Subsystem..." }
		: (sys == System::Input)
		? FlagAndName{ SDL_INIT_EVENTS, "SDL Input Subsystem..." }
		: (sys == System::Sound)
		? FlagAndName{ SDL_INIT_AUDIO, "SDL Audio Subsystem..." }
		: FlagAndName{ 0, "Unknown SDL Subsystem!" };
}



constexpr size_t toSizeT(SdlSystem::System sys) noexcept {
	return static_cast<size_t>(sys);
}




}
