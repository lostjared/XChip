#include <XChip/CpuManager.h>
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
template<class T>
static size_t get_error_flag_offs(const T*const memory) noexcept;
template<class T>
void* get_error_flag_addr(T*const memory) noexcept;







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





bool CpuManager::ResizeMemory(const std::size_t size)
{
	void* const errorFlag = get_error_flag_addr(_cpu.memory);

	if ( !realloc_cpu_arr(_cpu.memory, size)) 
	{
		LOGerr("Cannot reallocate Cpu memory to size: "_s + std::to_string(size));
		return false;
	}

	if(errorFlag != nullptr)
		PlaceErrorFlag(errorFlag);


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




bool CpuManager::SetGfx(const std::size_t size)
{
	if ( !alloc_cpu_arr(_cpu.gfx, size) )
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + std::to_string(size));
		return false;
	}

	return true;
}



void CpuManager::SetFont(const uint8_t* font, const size_t size)
{
	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::SetFont: null Cpu::memory");
	ASSERT_MSG(arr_size(_cpu.memory) > size,
		"CpuManager::SetFont: font size greater than Cpu::memory");

	memcpy(_cpu.memory, font, size);
}




bool CpuManager::LoadRom(const char* fileName, const size_t at)
{

	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::LoadRom: null Cpu::memory");
	ASSERT_MSG(arr_size(_cpu.memory) > at,
		"CpuManager::LoadRom: parameter 'at' greater than Cpu::memory");


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
	return arr_size(_cpu.memory);
}




size_t CpuManager::GetRegistersSize() const
{
	return arr_size(_cpu.registers);
}



size_t CpuManager::GetStackSize() const 
{
	return arr_size(_cpu.stack);
}



size_t CpuManager::GetGfxSize() const 
{
	return arr_size(_cpu.gfx);
}





void CpuManager::PlaceErrorFlag(void* addr)
{
	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::PlaceErrorFlag: null Cpu::memory");

	const auto flagOffs = get_error_flag_offs(_cpu.memory);
	InsertByte(0xFF, flagOffs - sizeof(uint8_t));
	InsertAddress(addr, flagOffs);
}


// static
void CpuManager::SetErrorFlag(Cpu& _cpu, const bool val)
{
	ASSERT_MSG(_cpu.memory != nullptr,
		"CpuManager::SetErrorFlag: null Cpu::memory");

	const auto flagOffs = get_error_flag_offs(_cpu.memory);
	if (_cpu.memory[flagOffs - sizeof(uint8_t)] == 0xFF)
		*reinterpret_cast<bool*&>(_cpu.memory[flagOffs]) = val;
}







template<class T>
static bool __alloc_arr(T*& arr, const size_t size) noexcept;
template<class T>
static bool __realloc_arr(T*& arr, const size_t size) noexcept;


template<class T>
static bool alloc_cpu_arr(T*& arr, const size_t size) noexcept
{
	if (size == arr_size(arr))
		return true;

	else if (arr != nullptr)
		free_arr(arr);

	
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




template<class T>
static size_t get_error_flag_offs(const T*const memory) noexcept
{
	return arr_size(memory) - sizeof(uintptr_t) - 1;
}



template<class T>
void* get_error_flag_addr(T*const memory) noexcept
{
	const auto flagOffs = get_error_flag_offs(memory);
	if (memory[flagOffs - sizeof(uint8_t)] == 0xFF)
		return reinterpret_cast<void*&>(memory[flagOffs]);

	return nullptr;
}





}
