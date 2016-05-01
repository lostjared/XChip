#include <csignal>
#include <iostream>

#include <XChip/Core/Emulator.h> 
#include <XChip/Media/SDLMedia.h>
#include <XChip/Utility/Memory.h>



static xchip::Emulator emu;


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




	if (argc < 2) {
		std::cout << "No game to load..." << std::endl;
		return EXIT_SUCCESS;
	}




	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;


	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	
	if (!emu.Initialize(std::move(render), std::move(input), std::move(sound)))
		return EXIT_FAILURE;

	if (!emu.LoadRom(argv[1]))
		return EXIT_FAILURE;



	if(signal(SIGINT, [](int signum)
	{
		std::cout << "Received signal: " << signum << std::endl;
		std::cout << "Closing Application!" << std::endl;
		emu.SetExitFlag(true);
	}) == SIG_ERR )
	{
		std::cout << "Could not intall signal handler!" << std::endl;
		return EXIT_FAILURE;
	}


	while (!emu.GetExitFlag())
	{
		emu.UpdateSystems(); 
		emu.HaltForNextFlag();		
		if (emu.GetInstrFlag()) 			
			emu.ExecuteInstr();
		if (emu.GetDrawFlag())
			emu.Draw();

	}





	return EXIT_SUCCESS;


}














