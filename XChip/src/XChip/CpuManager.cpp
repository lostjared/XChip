#include <algorithm>
#include <XChip/CpuManager.h>
#include <XChip/Utility/Log.h>
namespace xchip {

using namespace xchip::utility;



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

void CpuManager::Dispose() noexcept
{
	delete[] _cpu.gfx;
	delete[] _cpu.stack;
	delete[] _cpu.registers;
	delete[] _cpu.memory;
}


bool CpuManager::SetMemory(const std::size_t size)
{
	if (_cpu.memory != nullptr)
		delete[] _cpu.memory;

	_cpu.memory = new(std::nothrow) uint8_t[size];

	if (_cpu.memory == nullptr)
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	std::fill_n(_cpu.memory, size, 0);
	return true;
}



bool CpuManager::SetRegisters(const std::size_t size)
{
	if (_cpu.registers != nullptr)
		delete[] _cpu.stack;

	_cpu.registers = new(std::nothrow) uint8_t[size];

	if (_cpu.registers == nullptr)
	{
		LOGerr("Cannot allocate Cpu registers size: "_s + size);
		return false;
	}

	std::fill_n(_cpu.registers, size, 0);
	return true;
}


bool CpuManager::SetStack(const std::size_t size)
{
	if (_cpu.stack != nullptr)
		delete[] _cpu.stack;

	_cpu.stack = new(std::nothrow) uint16_t[size];

	if (_cpu.stack == nullptr)
	{
		LOGerr("Cannot allocate Cpu stack size: "_s + size);
		return false;
	}

	std::fill_n(_cpu.stack, size, 0);
	return true;
}


bool CpuManager::SetGfx(const std::size_t size)
{
	if (_cpu.gfx != nullptr)
		delete[] _cpu.gfx;

	_cpu.gfx = new(std::nothrow) uint32_t[size];

	if (_cpu.gfx == nullptr)
	{
		LOGerr("Cannot allocate Cpu memory size: "_s + size);
		return false;
	}

	std::fill_n(_cpu.gfx, size, 0);
	return true;
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




}
