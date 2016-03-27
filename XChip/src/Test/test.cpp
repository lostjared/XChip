#include <iostream>
#include <XChip/Emulator.h>
#include <XChip/Utility/Log.h>
#include <XChip/SDL/SdlRender.h>
#include <XChip/SDL/SdlInput.h>

int main(int argc, char** argv)
{
	using namespace xchip;
	using namespace utility;
	
	if (argc < 1) {
		utility::LOG("No game to load...");
		return 0;
	}

	atexit([]() {std::cin.ignore(); });

	
	static xchip::Emulator emulator;

	if (!emulator.Initialize(new(std::nothrow) SdlRender(), new(std::nothrow) SdlInput()))
		return 1;

	if (!emulator.LoadRom("TESTGAME"))
		return 1;

	while (! emulator.GetExitFlag())
	{
		emulator.HaltForNextFlag();
		emulator.UpdateSystems();
		if (emulator.GetInstrFlag())
			emulator.ExecuteInstr();
		if (emulator.GetDrawFlag())
			emulator.Draw();
	}


	return 0;


}


