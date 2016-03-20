#include <algorithm>
#include <XChip/CpuManager.h>
#include <XChip/Utility/Log.h>
namespace xchip {
using namespace xchip::utility;

template<class T>
inline void clean_arr(T* arr, size_t sz)
{
	if(arr != nullptr)
		std::fill_n(arr, sz, 0);
}

template<class T>
inline bool allocate_arr(T* arr, size_t sz)
{
	if (arr != nullptr)
		delete[] arr;

	arr = new(std::nothrow) T[sz];

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

	_memorySz    = 0;
	_registersSz = 0;
	_stackSz     = 0;
	_gfxSz       = 0;
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
	if(size == _memorySz) 
		return true;

	else if (! allocate_arr(_cpu.memory, size)) 
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		_memorySz = 0;
		return false;
	}

	_memorySz = size;
	return true;
}



bool CpuManager::SetRegisters(const std::size_t size)
{
	if(size == _registersSz) 
		return true;

	else if ( ! allocate_arr(_cpu.registers, size) )
	{
		LOGerr("Cannot allocate Cpu registers size: "_s + size);
		_registersSz = 0;
		return false;
	}

	_registersSz = size;
	return true;
}


bool CpuManager::SetStack(const std::size_t size)
{
	if(size == _stackSz) 
		return true;

	else if ( ! allocate_arr(_cpu.stack, size) )
	{
		LOGerr("Cannot allocate Cpu stack size: "_s + size);
		_stackSz = 0;
		return false;
	}

	_stackSz = size;
	return true;
}


bool CpuManager::SetGfx(const std::size_t size)
{
	if(_gfxSz == size)
		return true;

	else if (! allocate_arr(_cpu.gfx, size) )
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		_gfxSz = 0;
		return false;
	}

	_gfxSz = size;
	return true;
}




void CpuManager::cleanMemory()
{
	
	clean_arr(_cpu.memory, _memorySz);
}

void CpuManager::cleanRegisters()
{
	
	clean_arr(_cpu.registers, _registersSz);
}

void CpuManager::cleanStack()
{
	clean_arr(_cpu.stack, _stackSz);
}

void CpuManager::cleanGfx()
{
	clean_arr(_cpu.gfx, _gfxSz);
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
