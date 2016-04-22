#include <wXChip/RunEmulator.h>
#include <XChip/Utility/Timer.h>



void XChipEmulator::Stop()
{
	using namespace xchip::utility;
	using namespace xchip::utility::literals;
	
	_run = false;
	while(_isRunning != false) 
		Timer::Halt(150_milli);
}



void XChipEmulator::Run(XChipEmulator* xchipEmu)
{

	xchipEmu->_run = true;
	xchipEmu->_isRunning = true;


	auto& emu = xchipEmu->GetEmulator();

	while(!emu.GetExitFlag() && xchipEmu->_run)
	{
		emu.UpdateSystems();
		emu.HaltForNextFlag();
		if(emu.GetInstrFlag())
			emu.ExecuteInstr();
		if(emu.GetDrawFlag())
			emu.Draw();
	}

	xchipEmu->_isRunning = false;
}