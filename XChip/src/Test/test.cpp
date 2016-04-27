#include <iostream>
// first choose an API to controll the internal emulator
// default is Emulator class
#include <XChip/Core/Emulator.h> 


// Then choose your render / input / sound  APIs
// we already have SDL complete
#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>


#include <XChip/Utility/Memory.h> // make_unique as C++14

#if defined(__APPLE__) || defined(__linux__)
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<cstdio>

bool stop_prog = false;

void  Quit_handler(int sig)
{
	stop_prog = true;
	std::cout << "Quit..\n";
}

#endif


// ok . now lets run it

int main(int argc, char **argv)
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;

	
	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	act.sa_handler = &Quit_handler;
	if (sigaction(SIGTERM, &act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}


	if (argc < 2) {
		std::cout << "No game to load..." << std::endl;
		return EXIT_SUCCESS;
	}

	
	
	// lets create our  objects
	
	std::unique_ptr<Emulator> emu;

	/*
	
	we don't need to write all this
	
	std::unique_ptr<xchip::iRender> render;
	std::unique_ptr<xchip::iInput> input;
	std::unique_ptr<xchip::iSound> sound;
	
	*/

	// lets use the XChip type alias instead:
	
	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;


	// we need only worry about exception on
	// allocation with make_unique or new.
	// none of the Core functions will throw.
	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
		emu = make_unique<Emulator>();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// let the Emulator class take care of initializing the Media interfaces...	
	if (!emu->Initialize(std::move(render), std::move(input), std::move(sound)))
	{
		// could not be initialized
		return EXIT_FAILURE;
	}


	std::cout << "\t----- EMULATOR SETTINGS -----" << std::endl;


	// ok, now render, input, sound (the media interfaces) are initialized with the emulator's
	// defaults. but we can modify or use it, by gets.
	
	std::cout << "Default Sound countdown freq: " << emu->GetSound()->GetCountdownFreq() << std::endl;
	
	emu->GetSound()->SetCountdownFreq(35.88f);
	
	std::cout << "new Sound countdown freq: " << emu->GetSound()->GetCountdownFreq() << std::endl;
	
	// you can also:

	// emu->GetInput()->  whatever
	// emu->GetRender()-> whatever



	// remember to not use the old unique_ptr
	///render->DrawBuffer(); // ERROR we've moved it to the emulator

	// but we can get it back if we want
	render = emu->SwapRender(nullptr); 
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
	emu->SetRender(std::move(render));
	emu->CleanFlags();


	// ok, now you may want to set some
	// emulator settings
	// lets show the default FPS and CPU Frequency
	std::cout << "Default FPS: " << emu->GetFps() << std::endl;
	std::cout << "Default CPU Freq: " << emu->GetCpuFreq() << std::endl;

	emu->SetFps(120); // I want 120 fps
	emu->SetCpuFreq(485); // I want 485 instructions per second

	std::cout << "New FPS: " << emu->GetFps() << std::endl;
	std::cout << "New CPU Freq: " << emu->GetCpuFreq() << std::endl;




	// before running the emulator
	// we need to load a game
	if (!emu->LoadRom(argv[1]))
	{
		// could not load this rom
		return EXIT_FAILURE;
	}




	// finally the simple main loop

	while (!emu->GetExitFlag() && stop_prog == false)
	{
		emu->UpdateSystems(); // update window events / input events / timers / flags
		emu->HaltForNextFlag(); // sleep until instrFlag or drawFlag is TRUE
		
		if (emu->GetInstrFlag()) // if instrFLag is true, is time to execute one instruction
			emu->ExecuteInstr();
		if (emu->GetDrawFlag()) // if drawFlag is true, is time to the frame
			emu->Draw();

	}








}














