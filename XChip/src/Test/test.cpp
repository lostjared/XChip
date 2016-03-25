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
		LOGerr("Could not allocate memory properly...");
		return 0;
	}


	std::cout << "size of Chip8-Cpu memory: "    << manager->GetMemorySize()    << std::endl;
	std::cout << "size of Chip8-Cpu registers: " << manager->GetRegistersSize() << std::endl;
	std::cout << "size of Chip8-Cpu stack: "     << manager->GetStackSize()     << std::endl;
	std::cout << "size of Chip8-Cpu gfx: "       << manager->GetGfxSize()       << std::endl;


	if( ! manager->LoadRom(argv[1]) )
	{
		LOGerr("Failed to load rom...");
		return 0;
	}

	static bool exit = false;

	
	
	manager->SetRender(new(std::nothrow) SdlRender());
	manager->SetInput(new(std::nothrow) SdlInput());
	
	if( !manager->GetRender() || !manager->GetInput() )
	{
		LOGerr("could not allocate Render/Input...");
		return 0;
	}
	
	auto chip = &manager->GetCpu();	

	if(!chip->render->Initialize(64, 32) 
		|| !chip->input->Initialize() )
	{
		return 0;
	}

	chip->render->SetBuffer(chip->gfx);
	chip->render->SetWinCloseCallback(&exit, [](const void* exit){*(bool*)exit = true;});
	



	chip->input->SetEscapeKeyCallback( &exit, [](const void* exit) 
	{ 
		
		*(bool*)exit = true;

	});

	chip->input->SetResetKeyCallback(manager, [](const void* manager)
	{
		((CpuManager*)manager)->GetCpu().pc = 0x200;
		((CpuManager*)manager)->GetCpu().sp = 0;
		((CpuManager*)manager)->CleanRegisters();
		((CpuManager*)manager)->CleanStack();
		((CpuManager*)manager)->CleanGfx();

	});


	Timer fps( 1_sec / 60 );
	Timer instr( 680_hz );
	Timer timers( 60_hz );


	// [addr addr exit]-> [ addr exit ] - 8 bytes - [addr render]
	static void* exit_and_render[4] = { &exit, chip->render, &fps, &instr };

	chip->input->SetWaitKeyCallback((void*) exit_and_render, [](const void* args)
	{
		auto exit = (bool*) *((void**)args);
		auto render = (SdlRender*) *(((void**)args) + 1);
		auto fps = (Timer*) *(((void**)args) + 2);
		auto instr = (Timer*) *(((void**)args) + 3);

		do 
		{
			
			if(render->UpdateEvents()) 
			{ 
				if(*exit) return false; 
			}

			if(fps->Finished()) 
			{
				render->DrawBuffer();
				fps->Start();
			}

		}while(!instr->Finished());

		instr->Start();
		return true;
	});



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


