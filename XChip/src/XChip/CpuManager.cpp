#include <cstring>

#include <XChip/CpuManager.h>
#include <XChip/Interfaces/iRender.h>
#include <XChip/Interfaces/iInput.h>
#include <XChip/Interfaces/iSound.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Alloc.h>


namespace xchip {


using namespace xchip::utility;
using namespace xchip::utility::literals;

template<class T>
static bool alloc_cpu_arr(T*& arr, const size_t size);
template<class T>
static void free_cpu_arr(T*& arr);

 

CpuManager::CpuManager() noexcept
{
	LOG("Creating CpuManager object...");
	// init all members to 0
	memset(&_cpu, 0, sizeof(Cpu)); 
}



CpuManager::~CpuManager()
{
	this->Dispose();
	LOG("Destroying CpuManager object...");
}




void CpuManager::Dispose() noexcept
{
	free_cpu_arr(_cpu.gfx);
	free_cpu_arr(_cpu.stack);
	free_cpu_arr(_cpu.registers);
	free_cpu_arr(_cpu.memory);
}


bool CpuManager::SetMemory(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.memory, size)) 
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	return true;
}



bool CpuManager::SetRegisters(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.registers, size) ) 
	{
		LOGerr("Cannot allocate Cpu registers size: "_s + size);
		return false;
	}

	return true;
}


bool CpuManager::SetStack(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.stack, size) )
	{
		LOGerr("Cannot allocate Cpu stack size: "_s + size);
		return false;
	}

	return true;
}


bool CpuManager::SetGfx(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.gfx, size) )
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	return true;
}

void CpuManager::SetFont(const uint8_t* font, const size_t size)
{
	if(_cpu.memory)
		std::copy_n(font, size, _cpu.memory);
}




bool CpuManager::LoadRom(const char* fileName, const size_t at)
{
	LOG("Loading "_s + fileName);
	auto *const file = std::fopen(fileName, "rb");

	if (!file)
	{
		LOGerr("Error at opening ROM file, interrupting Chip8 instance.");
		return false;
	}

	const auto fileClose = make_scope_exit([file]() { std::fclose(file); });

	// get file size
	std::fseek(file, 0, SEEK_END);
	const auto fileSize = static_cast<size_t>(std::ftell(file));
	std::fseek(file, 0, SEEK_SET);

	// check if file size will not overflow emulated memory size
	if (fileSize > get_arr_size(_cpu.memory) - at) 
	{
		LOGerr("Error, ROM size not compatible, interrupting Chip8 instance.");
		return false;
	}

	std::fread(_cpu.memory + at, 1, fileSize, file);
	LOG("Load Done!");
	return true;
}




void CpuManager::CleanMemory()
{
	arr_zero(_cpu.memory);
}


void CpuManager::CleanRegisters()
{
	arr_zero(_cpu.registers);
	_cpu.I = 0;
	_cpu.delayTimer = 0;
	_cpu.soundTimer = 0;
}


void CpuManager::CleanStack()
{
	arr_zero(_cpu.stack);
	_cpu.sp = 0;
}


void CpuManager::CleanGfx()
{
	arr_zero(_cpu.gfx);
}




iRender* CpuManager::SwapRender(iRender* render)
{
	auto* const ret = _cpu.render;
	_cpu.render = render;
	return ret;
}



iInput* CpuManager::SwapInput(iInput* input)
{
	auto* const ret = _cpu.input;
	_cpu.input = input;
	return ret;
}



iSound* CpuManager::SwapSound(iSound* sound)
{
	auto* const ret = _cpu.sound;
	_cpu.sound = sound;
	return ret;
}




size_t CpuManager::GetMemorySize() const
{
	return get_arr_size(_cpu.memory);
}



size_t CpuManager::GetRegistersSize() const
{
	return get_arr_size(_cpu.registers);
}


size_t CpuManager::GetStackSize() const 
{
	return get_arr_size(_cpu.stack);
}


size_t CpuManager::GetGfxSize() const 
{
	return get_arr_size(_cpu.gfx);
}





template<class T>
static bool alloc_cpu_arr(T*& arr, const size_t size)
{
	if (size == get_arr_size(arr))
		return true;

	else if (arr != nullptr)
		free_arr(arr);

	arr = (T*)alloc_arr(size * sizeof(T));

	if (arr == nullptr)
		return false;

	std::fill_n(arr, size, 0);
	return true;
}


template<class T>
static void free_cpu_arr(T*& arr)
{
	free_arr(arr);
	arr = nullptr;
}







}
