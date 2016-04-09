#include <SDL2/SDL.h>
#include <XChip/SDL_MEDIA/SdlSystem.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Traits.h>
#include <XChip/Utility/Log.h>

 
namespace xchip {

template<class T>
constexpr size_t toSizeT(const T x) noexcept {
	return static_cast<size_t>(x);
}


SDL_Event g_sdlEvent;
int SdlSystem::s_nSystems[3] = { 0, 0, 0 };
bool SdlSystem::s_SubSystems[3] = { false, false, false };



SdlSystem::SdlSystem(const System sys)
	: _sys(sys)
{
	++s_nSystems[toSizeT(_sys)];
}



SdlSystem::~SdlSystem()
{
	using namespace utility;
	--s_nSystems[toSizeT(_sys)];
	
	// if this is the last instance of this System (_sys)
	// and Sdl Subsystem of (_sys) is On, then Close it
	if (!s_nSystems[toSizeT(_sys)]
		&& s_SubSystems[toSizeT(_sys)])
	{
		const Uint32 flags = [](const System sys) -> Uint32
		{
			switch (sys)
			{
				case System::Render:
					LOG("Closing SDL Video SubSystem...");
					return SDL_INIT_VIDEO;
				case System::Input:
					LOG("Closing SDL Input SubSystem...");
					return SDL_INIT_EVENTS;
				case System::Sound:
					LOG("Closing SDL Audio SubSystem...");
					return SDL_INIT_AUDIO;
				default:
					return 0;
			}
		}(_sys);
		
		SDL_QuitSubSystem(flags);
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
		const Uint32 flags = [](const System sys)  -> Uint32
		{
			switch (sys)
			{
				case System::Render:
					LOG("Initializing SDL Video SubSystem...");
					return SDL_INIT_VIDEO;
				case System::Input:
					LOG("Initializing SDL Input SubSystem...");
					return SDL_INIT_EVENTS;
				case System::Sound:
					LOG("Initializing SDL Audio SubSystem...");
					return SDL_INIT_AUDIO;
				default:
					return 0;
			}
		}(_sys);

		if (SDL_InitSubSystem(flags) != SDL_FALSE)
		{
			LOGerr("Could not initialize SDL SubSystem: "_s + SDL_GetError());
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














}
