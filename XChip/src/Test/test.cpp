#include <iostream>
#include <XChip/CpuManager.h>
#include <XChip/SDL/SdlRender.h>
#include <XChip/SDL/SdlInput.h>
#include <XChip/Utility/Log.h>

int main(int argc, char** argv)
{
	using namespace xchip;

	if (argc < 2) {
		utility::LOG("No game to load...");
	}
	

	CpuManager manager;

	manager.SetMemory(0xFFFF);
	manager.SetRegisters(16);
	manager.SetStack(16);
	manager.SetGfx(64 * 32);


	std::cout << "size of Chip8-Cpu memory: " << manager.GetMemorySize() << std::endl;
	std::cout << "size of Chip8-Cpu registers: " << manager.GetRegistersSize() << std::endl;
	std::cout << "size of Chip8-Cpu stack: " << manager.GetStackSize() << std::endl;
	std::cout << "size of Chip8-Cpu gfx: " << manager.GetGfxSize() << std::endl;


	manager.LoadRom(argv[1]);

	bool exit = false;

	auto render = new SdlRender();
	auto input = new SdlInput();

	render->SetWinCloseCallback([](const void* exit) { *(bool*)exit = true; }, &exit);
	input->SetWaitKeyCallback([](const void* render) 
	{ 
		if(((SdlRender*)render)->UpdateEvents()) 
			return false; 

		return true;
	}, &render);



	while (!exit)
	{
	
	}









	return 0;


}


