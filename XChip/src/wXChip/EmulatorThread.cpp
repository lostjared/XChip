#include <wXChip/EmulatorThread.h>
#include <XChip/Utility/Timer.h>



void EmulatorThread::Stop()
{
	using namespace xchip::utility;
	using namespace xchip::utility::literals;

	if(_isRunning)
	{	
		_emu.SetExitFlag(true);
		while(_isRunning != false) 
			Timer::Halt(150_milli);
	}

	Timer::Halt(100_milli);
}



void EmulatorThread::Run()
{

	
	static const auto loop = [] ( EmulatorThread * emuTr )
	{
		emuTr->_isRunning = true;


		auto& emu = emuTr->GetEmulator();

		emu.GetRender()->ShowWindow();

		while(!emu.GetExitFlag())
		{
			emu.UpdateSystems();
			emu.HaltForNextFlag();
			if(emu.GetInstrFlag())
				emu.ExecuteInstr();
			if(emu.GetDrawFlag())
				emu.Draw();
		}
		
		emu.GetRender()->HideWindow();
		emuTr->_isRunning = false;

	};

	_tr = std::thread(loop, this);
	_tr.detach();
}
