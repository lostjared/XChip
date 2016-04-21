#ifndef __RUNEMULATOR__H_
#define __RUNEMULATOR__H_

#include <XChip/Utility/Memory.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Core/Emulator.h>
#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>


class RunEmulator
{
public:
	
	RunEmulator();
	~RunEmulator();
	
	xchip::Emulator emu;
	bool load(const std::string &text);
	void init();
	void stop();
	void update();
	
	
private:
	bool closing;
};





#endif
