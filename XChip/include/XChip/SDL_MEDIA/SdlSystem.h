#ifndef _XCHIP_SDLSYSTEM_H_
#define _XCHIP_SDLSYSTEM_H_
union SDL_Event;
 
namespace xchip {


class SdlSystem
{
public:
	SdlSystem(const SdlSystem&) = delete;
	SdlSystem& operator=(const SdlSystem&) = delete;

protected:
	enum class System { Render, Input, Sound };
	SdlSystem(const System sys);
	virtual ~SdlSystem();

	bool InitSubSystem();
	static void PollEvent();

private:
	const System _sys;
	static int s_nSystems[3];
	static bool s_SubSystems[3];
};











}
#endif
