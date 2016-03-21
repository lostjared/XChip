#include <iostream>
#include <XChip/CpuManager.h>
#include <XChip/Instructions.h>
#include <XChip/SDL/SdlRender.h>
#include <XChip/SDL/SdlInput.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>
int main(int argc, char** argv)
{
	using namespace xchip;
	using namespace utility;

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

	render->Initialize(64, 32);
	input->Initialize();
	
	manager.SetInput(input);
	manager.SetRender(render);


	render->SetWinCloseCallback([](const void* exit) { *(bool*)exit = true; }, &exit);
	input->SetWaitKeyCallback([](const void* render) 
	{ 
		if(((SdlRender*)render)->UpdateEvents()) 
			return false; 

		return true;
	}, &render);
	input->SetEscapeKeyCallback([](const void* exit) { *(bool*)exit = true; }, &exit);

	auto& _cpu = manager.GetCpu();
	
	render->SetBuffer(_cpu.gfx);




	
	Timer fps( 1_sec / 60 );
	Timer instr( 358_hz );
	Timer timers( 60_hz );

	while (!exit)
	{
		render->UpdateEvents();
		input->UpdateKeys();


		if(timers.Finished())
		{
			if(_cpu.delayTimer > 0)
				-- _cpu.delayTimer;
			if(_cpu.soundTimer > 0)
				-- _cpu.soundTimer;

			timers.Start();
		}


		if(instr.Finished())
		{
			_cpu.opcode = ( _cpu.memory[_cpu.pc] << 8 ) | _cpu.memory[_cpu.pc + 1];
			_cpu.pc += 2;
			instructions::instrTable[ (_cpu.opcode & 0xf000) >> 12 ](&_cpu);
			instr.Start();
		}

		if(fps.Finished())
		{
			render->DrawBuffer();
			fps.Start();
		}

	
	}









	return 0;


}


