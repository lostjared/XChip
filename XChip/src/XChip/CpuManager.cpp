#include <algorithm>
#include <XChip/CpuManager.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Alloc.h>

namespace xchip {
using namespace xchip::utility;

template<class T>
inline void clean_arr(T* arr)
{
	if(arr != nullptr)
		std::fill_n(arr, get_arr_size(arr), 0);
}

template<class T>
static bool allocate_arr(T*& arr, size_t sz)
{

	if (sz == get_arr_size(arr))
		return true;

	else if (arr != nullptr)
		delete[] arr;

	arr = new T[sz];

	if (arr == nullptr) 
		return false;

	std::fill_n(arr, sz, 0);
	return true;
}



CpuManager::CpuManager() noexcept
{
	_cpu.opcode = 0;
	_cpu.I      = 0;
	_cpu.sp     = 0;
	_cpu.pc     = 0x200;
	_cpu.memory    = nullptr;
	_cpu.registers = nullptr;
	_cpu.stack     = nullptr;
	_cpu.gfx       = nullptr;
}

CpuManager::~CpuManager()
{
	delete[] _cpu.gfx;
	delete[] _cpu.stack;
	delete[] _cpu.registers;
	delete[] _cpu.memory;
}




bool CpuManager::SetMemory(const std::size_t size)
{
	if (! allocate_arr(_cpu.memory, size)) 
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	return true;
}



bool CpuManager::SetRegisters(const std::size_t size)
{
	if ( ! allocate_arr(_cpu.registers, size) )
	{
		LOGerr("Cannot allocate Cpu registers size: "_s + size);
		return false;
	}

	return true;
}


bool CpuManager::SetStack(const std::size_t size)
{
	if ( ! allocate_arr(_cpu.stack, size) )
	{
		LOGerr("Cannot allocate Cpu stack size: "_s + size);
		return false;
	}

	return true;
}


bool CpuManager::SetGfx(const std::size_t size)
{
	if (! allocate_arr(_cpu.gfx, size) )
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	return true;
}

void CpuManager::SetFont(const uint8_t* font, const size_t size)
{
	if(_cpu.memory != nullptr)
		std::copy_n(font, size, _cpu.memory);
}



void CpuManager::cleanMemory()
{
	clean_arr(_cpu.memory);
}



void CpuManager::cleanRegisters()
{
	clean_arr(_cpu.registers);
}



void CpuManager::cleanStack()
{
	clean_arr(_cpu.stack);
}



void CpuManager::cleanGfx()
{
	clean_arr(_cpu.gfx);
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





const uint8_t* CpuManager::GetMemory() const
{
	return _cpu.memory;
}



const uint8_t* CpuManager::GetRegisters() const
{
	return _cpu.registers;
}



const uint16_t* CpuManager::GetStack() const
{
	return _cpu.stack;
}



const uint32_t* CpuManager::GetGfx() const
{
	return _cpu.gfx;
}



const Cpu& CpuManager::GetCpu() const
{
	return _cpu;
}



Cpu& CpuManager::GetCpu()
{
	return _cpu;
}











}
