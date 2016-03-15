#include <SDL2/SDL_events.h>
#include <Chip8/Utility/timer.h>


SDL_Event g_sdlEvent;
void UpdateSdlEvents()
{
	/* 
		prevent both SdlRender/Input from update
		at the same period of 256th of second
	*/
	static Timer eventTimer(1_sec / 256);
	if( eventTimer.Finished() ) {
		SDL_PollEvent(&g_sdlEvent);
		eventTimer.Start();
	}
}
