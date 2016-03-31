#ifndef _XCHIP_SDLMEDIA_H_
#define _XCHIP_SDLMEDIA_H_

namespace xchip {

class SdlMedia
{
protected:
	enum class System { Render, Input, Sound };
	SdlMedia(System sys);
	virtual ~SdlMedia();
	static void UpdateEvents();


private:
	System _sys;
	static int s_onSystems[3];
};











}
#endif
