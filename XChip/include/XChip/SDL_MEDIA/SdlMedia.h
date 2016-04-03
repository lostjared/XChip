#ifndef _XCHIP_SDLMEDIA_H_
#define _XCHIP_SDLMEDIA_H_

#ifdef _WIN32
#include "EXTERN_HEADERS/WIN/SDL2/SDL_events.h"
#else
#include <SDL2/SDL_events.h>
#endif

namespace xchip {



class SdlMedia
{
protected:
	enum class System { Render, Input, Sound };
	SdlMedia(const System sys);
	virtual ~SdlMedia();

	static inline const SDL_Event& GetEvent() { return s_events; }
	bool InitSubSystem();
	static void UpdateEvents();

private:
	const System _sys;
	static SDL_Event s_events;
	static int s_nSystems[3];
	static bool s_SubSystems[3];
};











}
#endif
