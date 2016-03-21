#include <iostream>
#include <XChip/CpuManager.h>

int main(void)
{

	using namespace xchip;

	int *x = new int[10];
	CpuManager manager;

	manager.SetMemory(0xFFFF);
	manager.SetRegisters(16);
	manager.SetStack(16);
	manager.SetGfx(64 * 32);


	std::cout << "size of Chip8-Cpu memory: " << manager.GetMemorySize() << std::endl;
	std::cout << "size of Chip8-Cpu registers: " << manager.GetRegistersSize() << std::endl;
	std::cout << "size of Chip8-Cpu stack: " << manager.GetStackSize() << std::endl;
	std::cout << "size of Chip8-Cpu gfx: " << manager.GetGfxSize() << std::endl;



	return 0;


}


