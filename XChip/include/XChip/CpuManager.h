#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include <cstddef>
#include "Utility/Timer.h"
#include "Cpu.h"

namespace xchip {




class CpuManager
{
public:
	CpuManager() noexcept;
	~CpuManager();


	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const uint16_t* GetStack() const;
	const uint32_t* GetGfx() const;
	const Cpu& GetCpu() const;
	Cpu& GetCpu();

	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfx(const size_t size);
	void SetFont(const uint8_t* font, const size_t size);

	void cleanMemory();
	void cleanRegisters();
	void cleanStack();
	void cleanGfx();
private:
	Cpu _cpu;
	size_t _memorySz;
	size_t _registersSz;
	size_t _stackSz;
	size_t _gfxSz;
};















} // xchip namespace

#endif
