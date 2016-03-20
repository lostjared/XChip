#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include <cstddef>
#include "Cpu.h"
_XCHIP_NAMESPACE_





class CpuManager
{
public:
	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const uint16_t* GetStack() const;
	const uint32_t* GetGfx() const;

protected:
	CpuManager() noexcept;
	void Dispose() noexcept;

	bool SetMemory(const std::size_t size);
	bool SetRegisters(const std::size_t size);
	bool SetStack(const std::size_t size);
	bool SetGfx(const std::size_t size);


	Cpu _cpu;
};

















_XCHIP_NAMESPACE_END
#endif
