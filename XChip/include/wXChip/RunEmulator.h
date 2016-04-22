#ifndef __RUNEMULATOR__H_
#define __RUNEMULATOR__H_
#include <XChip/Core/Emulator.h>


class XChipEmulator
{
	
public:
	xchip::Emulator& GetEmulator();
	void Stop();

	static void Run(XChipEmulator* xchipEmu);

private:
	xchip::Emulator _emu;
	volatile bool _run = false;
	volatile bool _isRunning = false;
	
};






inline xchip::Emulator& XChipEmulator::GetEmulator() { return _emu; }





#endif
