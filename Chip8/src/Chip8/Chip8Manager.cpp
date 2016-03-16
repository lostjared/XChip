#include <cstdio>
#include <algorithm>

#include <Chip8/Chip8Manager.h>
#include <Chip8/Interfaces/iRenderer.h>
#include <Chip8/Interfaces/iInput.h>
#include <Chip8/Utility/Log.h>




Chip8CpuManager::Chip8CpuManager() noexcept
	: m_cpu(nullptr),
	m_rom(nullptr),
	m_memorysz(0),
	m_romsz(0)
{
	LOG("Constructing Chip8Manager...");
}



Chip8CpuManager::~Chip8CpuManager()
{
	if (m_cpu != nullptr)
		this->Dispose();

	LOG("Destroying Chip8Manager...");
}



bool Chip8CpuManager::Initialize() noexcept
{
	LOG("Initializing Chip8Manager...");
	if (m_cpu != nullptr)
		this->Dispose();

	m_cpu = new(std::nothrow) Chip8Cpu();
	
	if (!m_cpu) {
		LOGerr("Failed to allocate memory for Chip8Cpu !");
		return false;
	}

	return true;
}




void Chip8CpuManager::Dispose() noexcept
{
	if (m_cpu != nullptr) 
	{
		delete m_cpu->input;
		delete m_cpu->render;
		delete[] m_cpu->gfx;
		delete[] m_cpu->stack;
		delete[] m_cpu->registers;
		delete[] m_cpu->memory;
		delete m_cpu;
		m_cpu = nullptr;
	}

	LOG("Disposing Chip8Manager...");
}






bool Chip8CpuManager::SetMemory(const std::size_t size)
{
	if (size < MEMORY_MIN)
		LOGerr("WARNING: Memory size: " << size << " is too low!");

	else if (m_cpu->memory != nullptr)
		delete[] m_cpu->memory;

	LOG("Setting Chip8 memory: " << size << " bytes...");
	m_cpu->memory = new(std::nothrow) uint8_t[size];
	
	if (!m_cpu->memory) {
		LOGerr("Failed to allocate Chip8 memory !");
		m_memorysz = 0;
		return false;
	}

	std::fill_n(m_cpu->memory, size, 0);
	m_memorysz = size;
	return true;
}




bool Chip8CpuManager::SetRegisters(const std::size_t size)
{
	if (size < REGISTERS_MIN)
		LOGerr("WARNING: " << size << " registers is too low!");

	else if (m_cpu->registers != nullptr)
		delete[] m_cpu->registers;

	LOG("Setting Chip8 registers: " << size << " - 8 bits registers...");
	m_cpu->registers = new(std::nothrow) uint8_t[size];
	
	if (!m_cpu->registers) {
		LOGerr("Failed to allocate Chip8 Registers !");
		return false;
	}

	std::fill_n(m_cpu->registers, size, 0);

	return true;
}





bool Chip8CpuManager::SetStack(const std::size_t size)
{
	if (size < STACK_MIN)
		LOGerr("WARNING: stack size " << size << " is too low!");

	else if (m_cpu->stack != nullptr)
		delete[] m_cpu->stack;

	LOG("Setting Chip8 stack: " << size * sizeof(uint16_t) << " bytes...");
	m_cpu->stack = new(std::nothrow) uint16_t[size];
	if (!m_cpu->stack) {
		LOGerr("Failed to allocate Chip8 Stack !");
		return false;
	}
	
	std::fill_n(m_cpu->stack, size, 0);

	return true;
}





bool Chip8CpuManager::SetGfx(const std::size_t size)
{
	if (size < GFX_MIN) 
		LOGerr("WARNING: GFX size " << size << " is too low!");

	else if (m_cpu->gfx != nullptr)
		delete[] m_cpu->gfx;

	LOG("Setting Chip8 GFX: " << sizeof(uint32_t) * size << " bytes...");
	m_cpu->gfx = new(std::nothrow) uint32_t[size];
	
	if (!m_cpu->gfx) {
		LOGerr("Failed to allocate Chip8 GFX !");
		return false;
	}

	std::fill_n(m_cpu->gfx, size, 0);

	return true;
}



void Chip8CpuManager::SetFont(const uint8_t* font, const std::size_t size)
{
	LOG("Loading Chip8 Font: " << size << " bytes...");
	std::copy_n(font, size, m_cpu->memory);
}




bool Chip8CpuManager::LoadRom(const char * fileName)
{
	LOG("Loading " << fileName);
	std::FILE *file = std::fopen(fileName, "rb");

	if (file == nullptr)
	{
		LOGerr("Error at opening file " << fileName << ", interrupting Chip8 instance.");
		return false;
	}

	// get file size
	std::fseek(file, 0, SEEK_END);
	m_romsz = static_cast<std::size_t>(std::ftell(file));
	std::fseek(file, 0, SEEK_SET);

	// check if file size will not overflow emulated memory size
	if ( m_romsz > m_memorysz - 0x200 )
	{
		LOGerr("Error, " << fileName << " size not compatible, interrupting Chip8 instance.");
		std::fclose(file);
		m_romsz = 0;
		return false;
	}

	m_rom = fileName;
	std::fread(m_cpu->memory + 0x200, 1, m_romsz, file);
	std::fclose(file);
	LOG("Load Done!");

	return true;
}






iRenderer* Chip8CpuManager::SwapRender(iRenderer* rend)
{
	auto ret = m_cpu->render;
	m_cpu->render = rend;
	return ret;
}

iInput* Chip8CpuManager::SwapInput(iInput* input)
{
	auto ret = m_cpu->input;
	m_cpu->input = input;
	return ret;
}
