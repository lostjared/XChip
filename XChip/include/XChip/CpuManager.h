#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include <cstddef>
#include "Cpu.h"
_XCHIP_NAMESPACE_





class CpuManager
{
protected:
	CpuManager() noexcept;
	void Dispose();
	bool SetMemory(const std::size_t size);
	bool SetRegisters(const std::size_t size);
	bool SetStack(const std::size_t size);
	bool SetGfx(const std::size_t size);


	Cpu _cpu;
};

















_XCHIP_NAMESPACE_END
#endif
