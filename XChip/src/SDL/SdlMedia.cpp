#include <SDL2/SDL.h>
#include <XChip/SDL/SdlMedia.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Traits.h>

namespace xchip {

SDL_Event g_sdlEvent;
bool SdlMedia::s_onDevices[3] = { false, false, false };


SdlMedia::SdlMedia(System sys)
	: _sys(sys)
{
	s_onDevices[utility::toUType(_sys)] = true;
}

SdlMedia::~SdlMedia()
{
	using namespace utility;
	s_onDevices[toUType(_sys)] = false;
	if (!s_onDevices[toUType(System::Render)]
		&& !s_onDevices[toUType(System::Input)]
		&& !s_onDevices[toUType(System::Sound)])
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
