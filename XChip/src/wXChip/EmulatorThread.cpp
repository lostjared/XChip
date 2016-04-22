#include <wXChip/EmulatorThread.h>
#include <XChip/Utility/Timer.h>



void EmulatorThread::Stop()
{
	using namespace xchip::utility;
	using namespace xchip::utility::literals;

	if(_isRunning)
	{	
		_run = false;
		while(_isRunning != false) 
			std::this_thread::yield();
	}

//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}



void EmulatorThread::Run()
{

	
	static const auto loop = [] ( EmulatorThread * emuTr )
	{
		emuTr->_isRunning = true;


		auto& emu = emuTr->GetEmulator();

		emu.GetRender()->ShowWindow();

		while(!emu.GetExitFlag() && emuTr->_run)
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


	_run = true;
	_tr = std::thread(loop, this);
	_tr.detach();
}
