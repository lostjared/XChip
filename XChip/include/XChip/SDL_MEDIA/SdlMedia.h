#ifndef _XCHIP_SDLMEDIA_H_
#define _XCHIP_SDLMEDIA_H_
union SDL_Event;

namespace xchip {



class SdlMedia
{
protected:
	enum class System { Render, Input, Sound };
	SdlMedia(const System sys);
	virtual ~SdlMedia();

	static const SDL_Event& GetEvent();
	bool InitSubSystem();
	static void UpdateEvents();

private:
	const System _sys;
	static int s_nSystems[3];
	static bool s_SubSystems[3];
};











}
#endif
