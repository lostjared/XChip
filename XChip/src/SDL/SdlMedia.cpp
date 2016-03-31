#include <SDL2/SDL.h>
#include <XChip/SDL/SdlMedia.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Traits.h>

namespace xchip {

SDL_Event g_sdlEvent;
int SdlMedia::s_onSystems[3] = { 0, 0, 0 };


SdlMedia::SdlMedia(System sys)
	: _sys(sys)
{
	++s_onSystems[utility::toUType(_sys)];
}

SdlMedia::~SdlMedia()
{
	using namespace utility;
	--s_onSystems[toUType(_sys)];

	if (!s_onSystems[toUType(System::Render)]
		&& !s_onSystems[toUType(System::Input)]
		&& !s_onSystems[toUType(System::Sound)])
	{
		SDL_Quit();
	}
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
