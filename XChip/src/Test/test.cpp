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
		return 0;
	}
	

	static CpuManager *manager = new(std::nothrow) CpuManager();

	if( !manager->SetMemory(0xFFFF) 
		|| !manager->SetRegisters(16) 
		|| !manager->SetStack(16) 
		|| !manager->SetGfx(64 * 32) )
	{
		utility::LOGerr("Could not allocate memory properly...");
	}


	std::cout << "size of Chip8-Cpu memory: "    << manager->GetMemorySize()    << std::endl;
	std::cout << "size of Chip8-Cpu registers: " << manager->GetRegistersSize() << std::endl;
	std::cout << "size of Chip8-Cpu stack: "     << manager->GetStackSize()     << std::endl;
	std::cout << "size of Chip8-Cpu gfx: "       << manager->GetGfxSize()       << std::endl;


	if( ! manager->LoadRom(argv[1]) )
	{
		utility::LOGerr("Failed to load game...");
		return 0;
	}

	static bool exit = false;

	
	
	manager->SetRender(new(std::nothrow) SdlRender());
	manager->SetInput(new(std::nothrow) SdlInput());
	
	if( !manager->GetRender() || !manager->GetInput() )
	{
		utility::LOGerr("could not allocate Render/Input...");
		return 0;
	}
	
	auto chip = &manager->GetCpu();	

	if(!chip->render->Initialize(64, 32) 
		|| !chip->input->Initialize() )
	{
		return 0;
	}

	chip->render->SetBuffer(chip->gfx);

	chip->render->SetWinCloseCallback([](const void* exit) 
	{ 
		*(bool*)exit = true; 
	}, &exit);
	
	// [addr addr exit]-> [ addr exit ] - 8 bytes - [addr render]
	void* exit_and_render[2] = { &exit, chip->render };

	chip->input->SetWaitKeyCallback([](const void* exit_and_render)
	{
		auto exit = *((bool**)exit_and_render);
		auto render = (iRender*) * (((char**)exit_and_render) + sizeof(void*));
		if(render->UpdateEvents()) { if(*exit) return false; }
		render->DrawBuffer();
		return true;
	}, (void*) exit_and_render);


	chip->input->SetEscapeKeyCallback([](const void* exit) 
	{ 
		
		*(bool*)exit = true;

	}, &exit);

	chip->input->SetResetKeyCallback([](const void* manager)
	{
		((CpuManager*)manager)->GetCpu().pc = 0x200;
		((CpuManager*)manager)->GetCpu().sp = 0;
		((CpuManager*)manager)->CleanRegisters();
		((CpuManager*)manager)->CleanStack();
		((CpuManager*)manager)->CleanGfx();

	}, manager);


	Timer fps( 1_sec / 60 );
	Timer instr( 680_hz );
	Timer timers( 60_hz );

	while (!exit)
	{
		chip->render->UpdateEvents();
		chip->input->UpdateKeys();

		if(timers.Finished())
		{
			if(chip->delayTimer > 0)
				-- chip->delayTimer;
			if(chip->soundTimer > 0)
				-- chip->soundTimer;

			timers.Start();
		}


		if(instr.Finished())
		{
			chip->opcode = ( chip->memory[chip->pc] << 8 )  | chip->memory[chip->pc + 1];
			chip->pc += 2;
			instructions::instrTable[ (chip->opcode & 0xf000) >> 12 ](chip);
			instr.Start();
		}

		if(fps.Finished())
		{
			chip->render->DrawBuffer();
			fps.Start();
		}	
	}

	delete manager;


	return 0;


}


