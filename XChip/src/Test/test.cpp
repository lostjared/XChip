
#if 0
#include <iostream>
#include <SDL2/SDL.h>
#undef main
#include <XChip/SDL_MEDIA/SdlSound.h>


int main()
{
	using namespace xchip;
	SdlSound sound;

	if (!sound.Initialize()) {
		std::cout << "could not initialize sound";
		return EXIT_FAILURE;
	}

	sound.Play(10);

	SDL_Delay(1000 * 11);

	return 0;
}

#endif










#if 1
#include <XChip/Emulator.h>
#include <XChip/Utility/Log.h>
#include <XChip/SDL_MEDIA/SdlRender.h>
#include <XChip/SDL_MEDIA/SdlInput.h>
#include <XChip/SDL_MEDIA/SdlSound.h>



int main(int argc, char** argv)
{
	using std::nothrow;
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	
	if (argc < 2) 
	{
		xchip::utility::LOG("No game to load...");
		return EXIT_FAILURE;
	}


	// pause at exit to output read in WINDOWS
	// cuz we might not be using cmd to call the program
	#ifdef _WIN32 
	std::atexit([]()
	{
		xchip::utility::LOG("PRESS ANY KEY TO EXIT");
		std::cin.ignore();
	});
	#endif

	static Emulator emulator;

	if (!emulator.Initialize(new(nothrow) SdlRender(), 
                             new(nothrow) SdlInput(),
                             new(nothrow) SdlSound()))
	{
		return EXIT_FAILURE;
	}

	else if (!emulator.LoadRom(argv[1]))
	{
		return EXIT_FAILURE;
	}

	// all good to run the main loop...
	while (! emulator.GetExitFlag())
	{
		emulator.HaltForNextFlag();
		emulator.UpdateSystems();
		
		if (emulator.GetInstrFlag())
			emulator.ExecuteInstr();
		
		if (emulator.GetDrawFlag())
			emulator.Draw();
	}



	return EXIT_SUCCESS;
}


#endif
