#include <XChip/Emulator.h>
#include <XChip/Utility/Log.h>
#include <XChip/SDL/SdlRender.h>
#include <XChip/SDL/SdlInput.h>
#include <XChip/SDL/SdlSound.h>




int main(int argc, char** argv)
{
	std::atexit([]() {std::cin.ignore(); });
	if (argc < 2) 
	{
		xchip::utility::LOG("No game to load...");
		return 0;
	}

	static xchip::Emulator emulator;
	


	if (!emulator.Initialize(new(std::nothrow) xchip::SdlRender(),
                             new(std::nothrow) xchip::SdlInput(), 
                             new(std::nothrow) xchip::SdlSound()))
	{
		return 1;
	}

	else if (!emulator.LoadRom(argv[1]))
	{
		return 1;
	}

	emulator.GetRender()->SetWinResizeCallback(nullptr, [](const void*) {std::cout << "RESIZED" << std::endl; });

	while (! emulator.GetExitFlag())
	{
		emulator.HaltForNextFlag();
		emulator.UpdateSystems();
		
		if (emulator.GetInstrFlag())
			emulator.ExecuteInstr();
		
		if (emulator.GetDrawFlag())
			emulator.Draw();
	}

	xchip::utility::LOG("PRESS ANY KEY TO EXIT");
	return 0;


}


