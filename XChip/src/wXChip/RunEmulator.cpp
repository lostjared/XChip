#include<wXChip/RunEmulator.h>

RunEmulator::RunEmulator()
{
	
	
}

RunEmulator::~RunEmulator()
{
	stop();
}


void RunEmulator::init()
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;
	
	
	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;
	
	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return;
	}
	
	if (!emu.Initialize(std::move(render), std::move(input), std::move(sound)))
	{
		return;
	}
	
	
	emu.GetSound()->SetCountdownFreq(35.88f);
	
	// you can also:
	
	// emu.GetInput()->  whatever
	// emu.GetRender()-> whatever
	
	
	
	// remember to not use the old unique_ptr
	///render->DrawBuffer(); // ERROR we've moved it to the emulator
	
	// but we can get it back if we want
	render = emu.SwapRender(nullptr);
	// get our render back and set emulators render to nullptr
	// note that setting some emulator media interface to null
	// will set ExitFlag on.
	
	
	// lets check our default game color
	auto color = render->GetColorFilter();
	std::cout << "Default Color Filter: " << color << std::endl;
	
	// lets set our game color RED
	color = { 255, 0, 0 };
	
	if(!render->SetColorFilter(color))
	{
		std::cout << "could not set new color filter" << std::endl;
	}
	else
	{
		color = render->GetColorFilter();
		std::cout << "New Color Filter: " << color << std::endl;
	}
	
	// don't forget to put the render back!!
	// but because we have seted it to null before
	// now we need to clean flags
	emu.SetRender(std::move(render));
	emu.CleanFlags();
	
	
	// ok, now you may want to set some
	// emulator settings
	// lets show the default FPS and CPU Frequency
	std::cout << "Default FPS: " << emu.GetFps() << std::endl;
	std::cout << "Default CPU Freq: " << emu.GetCpuFreq() << std::endl;
	
	emu.SetFps(120); // I want 120 fps
	emu.SetCpuFreq(485); // I want 485 instructions per second
	
	std::cout << "New FPS: " << emu.GetFps() << std::endl;
	std::cout << "New CPU Freq: " << emu.GetCpuFreq() << std::endl;
	closing = false;
	
}


void RunEmulator::stop()
{
	closing = true;
	
}

void RunEmulator::update() {
	while (!emu.GetExitFlag() && closing == false)
	{
		
		emu.UpdateSystems(); // update window events / input events / timers / flags
		emu.HaltForNextFlag(); // sleep until instrFlag or drawFlag is TRUE
		
		if (emu.GetInstrFlag()) // if instrFLag is true, is time to execute one instruction
			emu.ExecuteInstr();
		if (emu.GetDrawFlag()) // if drawFlag is true, is time to the frame
			emu.Draw();
		
		
	}
}

bool RunEmulator::load(const std::string &text) {
	return emu.LoadRom(text);
}

