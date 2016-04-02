#include <SDL2/SDL.h>
#include <XChip/SDL/SdlMedia.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Traits.h>
#include <XChip/Utility/Log.h>

namespace xchip {

SDL_Event g_sdlEvent;
int SdlMedia::s_nSystems[3] = { 0, 0, 0 };
bool SdlMedia::s_SubSystems[3] = { false, false, false };

SdlMedia::SdlMedia(System sys)
	: _sys(sys)
{
	++s_nSystems[utility::toUType(_sys)];
}

SdlMedia::~SdlMedia()
{
	using namespace utility;
	--s_nSystems[toUType(_sys)];

	// if this is the last instance of this System (_sys)
	// and Sdl Subsystem of (_sys) is On, then Close it
	if (!s_nSystems[toUType(_sys)]
		&& s_SubSystems[toUType(_sys)])
	{
		Uint32 flags;
		switch (_sys)
		{
			case System::Render:
				flags = SDL_INIT_VIDEO;
				LOG("Closing SDL Video Subsystem...");
				break;
			case System::Input:
				flags = SDL_INIT_EVENTS;
				LOG("Closing SDL Input Subsystem...");
				break;
			case System::Sound:
				flags = SDL_INIT_AUDIO;
				LOG("Closing SDL Audio Subsystem...");
				break;
		}

		SDL_QuitSubSystem(flags);
		s_SubSystems[toUType(_sys)] = false;
	}
}


bool SdlMedia::InitSubsystem()
{
	using namespace utility;
	using namespace utility::literals;

	// if the SDL Subsystem of (_sys) is off
	// then initialize it
	if( !s_SubSystems[toUType(_sys)] )
	{
		Uint32 flags;
		switch (_sys)
		{
			case System::Render:
				flags = SDL_INIT_VIDEO;
				LOG("Initializing SDL Video Subsystem...");
				break;
			case System::Input:
				flags = SDL_INIT_EVENTS;
				LOG("Initializing SDL Input Subsystem...");
				break;
			case System::Sound:
				flags = SDL_INIT_AUDIO;
				LOG("Initializing SDL Audio Subsystem...");
				break;
		}
		
		if (SDL_InitSubSystem(flags) != SDL_FALSE)
		{
			LOGerr("Could not initialize SDL Subsystem: "_s + SDL_GetError());
			return false;
		}

		s_SubSystems[toUType(_sys)] = true;
	}
	
	return true;
}

void SdlMedia::UpdateEvents()
{
	using namespace xchip::utility;
	using namespace xchip::utility::literals;
	// prevent both SdlRender/Input from update
	// at the same period of 256th of second
	static Timer eventTimer(256_hz);
	if (eventTimer.Finished()) 
	{
		SDL_PollEvent(&g_sdlEvent);
		eventTimer.Start();
	}
}














}
