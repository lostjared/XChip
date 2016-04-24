#include <wXChip/EmulatorThread.h>
#include <XChip/Utility/Timer.h>



void EmulatorThread::Stop()
{
	using namespace xchip::utility;
	using namespace xchip::utility::literals;

	if(_isRunning)
	{	
		_endTr = true;
		
		if (_tr.joinable())
			_tr.join();

		else
			while (_isRunning != false);
				Timer::Halt(150_milli);
	}

	Timer::Halt(100_milli);
}



void EmulatorThread::Run()
{

	
	static const auto loop = [this] ()
	{
		this->_isRunning = true;


		auto& emu = this->GetEmulator();

		emu.GetRender()->ShowWindow();

		while(!emu.GetExitFlag() && !this->_endTr)
		{
			emu.UpdateSystems();
			emu.HaltForNextFlag();
			if(emu.GetInstrFlag())
				emu.ExecuteInstr();
			if(emu.GetDrawFlag())
				emu.Draw();
		}
		
		emu.GetRender()->HideWindow();
		this->_isRunning = false;

	};

	_tr = std::thread(loop);
	_tr.detach();
}
