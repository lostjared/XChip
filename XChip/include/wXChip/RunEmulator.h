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



inline bool RunEmulator::isRunning() { return s_isRunning; }
inline void RunEmulator::stop() { s_close = true; }
inline void RunEmulator::unstop() { s_close = false; }
#endif
