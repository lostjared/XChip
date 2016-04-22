#ifndef _WXCHIP_EMULATORTHREAD_H_
#define _WXCHIP_EMULATORTHREAD_H_
#include <XChip/Core/Emulator.h>
#include <thread>

class EmulatorThread
{	
public:
	xchip::Emulator& GetEmulator();
	void Stop();
	void Run();
private:
	xchip::Emulator _emu;
	std::thread _tr;
	volatile bool _isRunning = false;
	
};







inline xchip::Emulator& EmulatorThread::GetEmulator() { return _emu; }





#endif
