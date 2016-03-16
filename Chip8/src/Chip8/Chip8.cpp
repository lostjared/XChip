#include <Chip8/Chip8.h>
#include <Chip8/Interfaces/iRenderer.h>
#include <Chip8/Interfaces/iInput.h>
#include <Chip8/Utility/Log.h>


Chip8Emu::~Chip8Emu()
{
	if (m_needToDispose)
		this->Dispose();

	LOG("Destroying Chip8Emu...");
}

bool Chip8Emu::Initialize() noexcept
{
	if (!Chip8CpuManager::Initialize())
		return false;

	else if (!SetMemory() || !SetRegisters()
		|| !SetStack() || !SetGfx()) {
		return false;
	}

	static uint8_t chip8Font[80]
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	SetFont(chip8Font, 80);


	InitRender(64, 32);
	InitInput();


}


void Chip8Emu::Dispose() noexcept
{
	Chip8CpuManager::Dispose();
	m_needToDispose = false;
}


void Chip8Emu::HaltForNextFlag() const
{
	if (!m_instrFlag && !m_drawFlag)
	{
		auto instrRmn = m_clocks.instr.GetRemain();
		auto drawRmn = m_clocks.frame.GetRemain();
		Timer::Halt(instrRmn < drawRmn ? instrRmn : drawRmn);
	}
}




void Chip8Emu::UpdateSystems()
{
	m_cpu->render->UpdateEvents();
	m_cpu->input->UpdateKeys();

}


void Chip8Emu::Draw()
{
	m_cpu->render->RenderBuffer();
	m_drawFlag = false;
}


void Chip8Emu::ExecuteNextOpcode()
{
	
}




void Chip8Emu::CleanFlags()
{
	m_instrFlag = false;
	m_drawFlag = false;
}


bool Chip8Emu::InitRender(const int w, const int h)
{
	if (!m_cpu->render) {
		LOGerr("Error: NULL iRenderer...");
		return false;
	}

	else if (m_cpu->render->IsInitialized()) {
		m_cpu->render->SetBuffer(m_cpu->gfx);
		return true;
	}
	
	else if (!m_cpu->render->Initialize(w, h))
		return false;

	m_cpu->render->SetBuffer(m_cpu->gfx);
}



bool Chip8Emu::InitInput()
{
	if (!m_cpu->input) {
		LOGerr("Error: NULL iRenderer...");
		return false;
	}

	else if (m_cpu->input->IsInitialized())
		return true;


	return m_cpu->input->Initialize();

}





int Chip8Emu::GetInstrPerSec() const
{
	return 0;
}

int Chip8Emu::GetFramesPerSec() const
{
	return 0;
}


void Chip8Emu::SetInstrPerSec(const int instr)
{


}

void Chip8Emu::SetFramesPerSec(const int frames)
{



}