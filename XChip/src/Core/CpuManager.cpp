#include <XChip/Core/CpuManager.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>
 
namespace xchip {



using namespace xchip::utility;
using namespace xchip::utility::literals;

template<class T>
static bool alloc_cpu_arr(T*& arr, const size_t size) noexcept;
template<class T>
static bool realloc_cpu_arr(T*& arr, const size_t size) noexcept;
template<class T>
static void free_cpu_arr(T*& arr) noexcept;





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
		LOGerr("Cannot allocate Cpu memory size: "_s + std::to_string(size));
		return false;
	}

	return true;
}



bool CpuManager::SetRegisters(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.registers, size)) 
	{
		LOGerr("Cannot allocate Cpu registers size: "_s + std::to_string(size));
		return false;
	}

	return true;
}


bool CpuManager::SetStack(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.stack, size) )
	{
		LOGerr("Cannot allocate Cpu stack size: "_s + std::to_string(size));
		return false;
	}

	return true;
}



bool CpuManager::SetGfx(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.gfx, size) )
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + std::to_string(size));
		return false;
	}

	return true;
}






bool CpuManager::ResizeMemory(const std::size_t size)
{

	if ( !realloc_cpu_arr(_cpu.memory, size)) 
	{
		LOGerr("Cannot reallocate Cpu memory to size: "_s + std::to_string(size));
		return false;
	}


	return true;
}



bool CpuManager::ResizeRegisters(const size_t size)
{
	if (!realloc_cpu_arr(_cpu.registers, size))
	{
		LOGerr("Cannot reallocate Cpu registers to size: "_s + std::to_string(size));
		return false;
	}

	return true;
}




bool CpuManager::ResizeStack(const size_t size)
{
	if (!realloc_cpu_arr(_cpu.stack, size))
	{
		LOGerr("Cannot reallocate Cpu stack to size: "_s + std::to_string(size));
		return false;
	}

	return true;
}



bool CpuManager::ResizeGfx(const size_t size)
{
	if (!realloc_cpu_arr(_cpu.gfx, size))
	{
		LOGerr("Cannot reallocate Cpu GFX to size: "_s + std::to_string(size));
		return false;
	}

	return true;
}



void CpuManager::LoadFont(const uint8_t* font, const size_t size, const size_t at)
{
	ASSERT_MSG(_cpu.memory != nullptr, "null Cpu::memory");
	ASSERT_MSG( (arr_size(_cpu.memory) - at) > size, "font size greater than Cpu::memory");
	memcpy(_cpu.memory + at, font, size);
}




bool CpuManager::LoadRom(const char* fileName, const size_t at)
{

	ASSERT_MSG(_cpu.memory != nullptr, "null Cpu::memory");
	ASSERT_MSG(arr_size(_cpu.memory) > at, "parameter 'at' greater than Cpu::memory");


	LOG("Loading "_s + fileName);
	auto *const file = std::fopen(fileName, "rb");

	if (!file)
	{
		LOGerr("Error at opening ROM file, interrupting Chip8 instance.");
		return false;
	}

	const auto fileClose = make_scope_exit([file]() noexcept { std::fclose(file); });

	// get file size
	std::fseek(file, 0, SEEK_END);
	const auto fileSize = static_cast<size_t>(std::ftell(file));
	std::fseek(file, 0, SEEK_SET);


	
	
	// check if file size will not overflow emulated memory size
	// careful to compare unsigned values, and subtracting them
	if ( (arr_size(_cpu.memory) - at) < fileSize)
	{
		LOGerr("Error, ROM size not compatible!");
		return false;
	}

	std::fread(_cpu.memory + at, 1, fileSize, file);
	LOG("Load Done!");
	return true;
}








iRender* CpuManager::SwapRender(iRender* render)
{
	ASSERT_MSG(render != _cpu.render, "trying to swap the same addresses");

	auto* const ret = _cpu.render;
	_cpu.render = render;
	return ret;
}



iInput* CpuManager::SwapInput(iInput* input)
{
	ASSERT_MSG(input != _cpu.input, "trying to swap the same addresses");

	auto* const ret = _cpu.input;
	_cpu.input = input;
	return ret;
}



iSound* CpuManager::SwapSound(iSound* sound)
{
	ASSERT_MSG(sound != _cpu.sound, "trying to swap the same addresses");

	auto* const ret = _cpu.sound;
	_cpu.sound = sound;
	return ret;
}







// local template functions
template<class T>
static bool __alloc_arr(T*& arr, const size_t size) noexcept;
template<class T>
static bool __realloc_arr(T*& arr, const size_t size) noexcept;


template<class T>
static bool alloc_cpu_arr(T*& arr, const size_t size) noexcept
{
	if (arr != nullptr)
	{
		if (size == arr_size(arr))
			return true;
		
		else
			free_arr(arr);
	}


	return __alloc_arr(arr, size);
}


template<class T>
static bool realloc_cpu_arr(T*& arr, const size_t size) noexcept
{
	if (arr == nullptr)
		return __alloc_arr(arr, size);

	else if(size == arr_size(arr))
		return true;
	
	return __realloc_arr(arr, size);
}



template<class T>
static void free_cpu_arr(T*& arr) noexcept
{
	if(arr != nullptr)
	{
		free_arr(arr);
		arr = nullptr;
	}

}




template<class T>
static bool __alloc_arr(T*& arr, const size_t size) noexcept
{
	arr = (T*)alloc_arr(sizeof(T) * size);
	
	if (!arr)
		return false;

	arr_zero(arr);
	return true;
}



template<class T>
static bool __realloc_arr(T*& arr, const size_t size) noexcept
{
	arr = (T*) realloc_arr(arr, sizeof(T) * size);
	return arr != nullptr;
}


















}
