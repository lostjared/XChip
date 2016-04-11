
// assert test
#if 0
#include <XChip/Emulator.h>
#include <XChip/Utility/Alloc.h>

int main(void)
{
	using xchip::CpuManager;
	CpuManager man;
	man.CleanMemory();

}

#endif




// normal run:
#if 1
#include <iostream>
#include <XChip/Emulator.h>
#include <XChip/SDL_MEDIA/SdlRender.h>
#include <XChip/SDL_MEDIA/SdlInput.h>
#include <XChip/SDL_MEDIA/SdlSound.h>
#include <XChip/Utility/Memory.h>

int main(int argc, char** argv)
{
	using std::move;
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::utility::make_unique; // nothrow version	

	if(argc < 2)
	{
		std::cout << "No game to load..." << std::endl;
		return EXIT_SUCCESS;
	}


	auto render = make_unique<SdlRender>();
	auto input = make_unique<SdlInput>();
	auto sound = make_unique<SdlSound>();


	Emulator emulator;
	if(!emulator.Initialize(move(render), move(input), move(sound)))
		return EXIT_FAILURE;
	
	if(!emulator.LoadRom(argv[1]))
		return EXIT_FAILURE;


	while(!emulator.GetExitFlag())
	{
		emulator.HaltForNextFlag();
		emulator.UpdateSystems();
		if(emulator.GetInstrFlag())
			emulator.ExecuteInstr();
		if(emulator.GetDrawFlag())
			emulator.Draw();
	}



	return EXIT_SUCCESS;
}







#endif






// DEMO:
#if 0
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
	using xchip::utility::make_unique; // xchip nothrow version of make_unique
	


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


	

	auto render = make_unique<SdlRender>();
	auto input = make_unique<SdlInput>();
	auto sound = make_unique<SdlSound>();

	Emulator emulator;

	// you must give the ownerty of the media interfaces
	// to the emulator, it won't accept raw pointers

	if (!emulator.Initialize(move(render), move(input), move(sound)))
	{
		return EXIT_FAILURE;
	}


	// we can use our media interfaces from the emulator
	auto rend = emulator.GetRender();
	if(rend->IsInitialized()) { /*... */  }
	// do whatever to the interface

	// want to do something specific to the child class ? dynamic cast it
	auto sdlRender = dynamic_cast<SdlRender*>(rend);
	if (sdlRender != nullptr) {
		//sdlRender->whatever...
	}

	// but do not delete raw pointers from emulator
	// delete rend; <- don't do it

	// if you want to own the object again
	// recover it by swap
	auto oldRend = emulator.SwapRender(nullptr);
	// now you can delete if you want, or just let the 
	// smart pointer take care of it.


	// you can set it back if u want... moving the unique pointer
	emulator.SetRender(move(oldRend));
	
	
	// and even if you swap without catching it
	// the smart pointer will take care of it and delete it
	emulator.SwapRender(nullptr);


	// by here the oldRend is now nullptr, lets reset it
	oldRend = make_unique<SdlRender>();

	
	// if the the new Render we inserted didn't initialized well or you seted a nullptr
	// the emulator's exitflag is set 
	if (emulator.GetExitFlag())
	{
		utility::LOGerr("new render not initialized trying again..");
		// give the old rend back to the emulator
		
		if (emulator.SetRender(move(oldRend)))
		{
			// initialized well, clean error flags
			emulator.CleanFlags();
		}

		else
		{
			// still didn't initialized, ok I'll give up
			return EXIT_FAILURE;
		}

	}

	// lets try load our rom
	if (!emulator.LoadRom(argv[1]))
	{
		return EXIT_FAILURE;
	}

	// ok, while (allgood) lets run this
	while (!emulator.GetExitFlag())
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





