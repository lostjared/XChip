#ifndef __RUNEMULATOR__H_
#define __RUNEMULATOR__H_
#include <XChip/Core/Emulator.h>


class RunEmulator
{
public:
	static std::unique_ptr<RunEmulator> create();
	~RunEmulator();
	
	bool load(const std::string &text);
	bool init();
	void update();

	static bool isRunning();
	static void stop();
	static void unstop();


private:
	RunEmulator();

	static volatile bool s_isRunning;
	static volatile bool s_close;
	xchip::Emulator emu;
	xchip::UniqueRender render;
	xchip::UniqueInput input;
	xchip::UniqueSound sound;
	
};



#endif
