#include <wXChip/RunEmulator.h>
#include <XChip/Utility/Memory.h>
#include <XChip/Utility/Log.h>
#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>


std::unique_ptr<RunEmulator> RunEmulator::create()
{
	if (!s_isRunning)
		return std::unique_ptr<RunEmulator>(new RunEmulator());

	return nullptr;
}



RunEmulator::RunEmulator()
{
	
}

RunEmulator::~RunEmulator()
{
	stop();
}


bool RunEmulator::init()
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;
	
	if (s_isRunning)
		return false;
	
	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}


	
	if (!emu.Initialize(std::move(render), std::move(input), std::move(sound)))
	{
		return false;
	}
	
	return true;
}



void RunEmulator::update() 
{
	s_isRunning = true;

	while (!emu.GetExitFlag() && s_close == false)
	{
		
		emu.UpdateSystems(); // update window events / input events / timers / flags
		emu.HaltForNextFlag(); // sleep until instrFlag or drawFlag is TRUE
		
		if (emu.GetInstrFlag()) // if instrFLag is true, is time to execute one instruction
			emu.ExecuteInstr();
		if (emu.GetDrawFlag()) // if drawFlag is true, is time to the frame
			emu.Draw();
	}

	s_isRunning = false;
}

bool RunEmulator::load(const std::string& romName) 
{
	return emu.LoadRom(romName);
}

