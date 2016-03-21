#include <SDL2/SDL_events.h>
#include <XChip/Utility/Timer.h>

namespace xchip {

SDL_Event g_sdlEvent;
void UpdateSdlEvents()
{
	using namespace xchip::utility;
	/*
	prevent both SdlRender/Input from update
	at the same period of 256th of second
	*/
	static Timer eventTimer(1_sec / 256);
	if (eventTimer.Finished()) {
		SDL_PollEvent(&g_sdlEvent);
		eventTimer.Start();
	}
}














}
