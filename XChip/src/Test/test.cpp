
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


#if 0
#include <XChip/Emulator.h>
#include <XChip/Utility/Log.h>
#include <XChip/SDL_MEDIA/SdlRender.h>
#include <XChip/SDL_MEDIA/SdlInput.h>
#include <XChip/SDL_MEDIA/SdlSound.h>



int main(int argc, char** argv)
{
	using std::make_unique;
	using std::move;
	using std::nothrow;
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;

	if (argc < 1) 
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


	if (!emulator.Initialize(make_unique<SdlRender>(), 
                             make_unique<SdlInput>(),
                             make_unique<SdlSound>()))
	{
		return EXIT_FAILURE;
	}
	else if (!emulator.LoadRom("ufo"))
	{
		return EXIT_FAILURE;
	}

	main_loop:
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

	
	auto newRend = make_unique<SdlRender>();
	auto newInput = make_unique<SdlInput>();
	auto newSound = make_unique<SdlSound>();

	emulator.Dispose();
	
	if (!emulator.Initialize(std::move(newRend), std::move(newInput), std::move(newSound)) )
		return EXIT_FAILURE;

	if (!emulator.LoadRom("ufo"))
		return EXIT_FAILURE;
	
	goto main_loop;


	return EXIT_SUCCESS;
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
	using namespace xchip;
	using std::move;
	using std::nothrow;
	using std::unique_ptr;
	using std::shared_ptr;

	if (argc < 2) 
	{
		utility::LOGerr("No games");
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


	// using C++14 make_unique
	using std::make_unique;

	if (!emulator.Initialize(make_unique<SdlRender>(), 
		                     make_unique<SdlInput>(), 
		                     make_unique<SdlSound>()))
	{
		return EXIT_FAILURE;
	}

	else if (!emulator.LoadRom(argv[1]))
	{
		return EXIT_FAILURE;
	}

	while(!emulator.GetExitFlag())
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
