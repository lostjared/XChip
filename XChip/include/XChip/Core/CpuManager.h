#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include "Cpu.h"
#include <XChip/Utility/Alloc.h>

 

namespace xchip {



class CpuManager
{
public:
	CpuManager() noexcept;
	~CpuManager();
	CpuManager(const CpuManager&) = delete;
	CpuManager& operator=(const CpuManager&) = delete;

	void Dispose() noexcept;

	size_t GetMemorySize() const;
	size_t GetRegistersSize() const;
	size_t GetStackSize() const;
	size_t GetGfxSize() const;
	bool GetErrorFlag() const;

	const iRender* GetRender() const;
	const iInput* GetInput() const;
	const iSound* GetSound() const;
	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const size_t* GetStack() const;
	const uint32_t* GetGfx() const;
	const Cpu& GetCpu() const;

	Cpu& GetCpu();
	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();

	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfx(const size_t size);
	bool ResizeMemory(const size_t size);
	bool ResizeRegisters(const size_t size);
	bool ResizeStack(const size_t size);
	bool ResizeGfx(const size_t size);
	void SetErrorFlag(const bool val);
	void SetPC(const size_t offset);
	void SetSP(const size_t offset);
	void SetFont(const uint8_t* font, const size_t size);
	bool LoadRom(const char* file, const size_t at);
	void SetRender(iRender* render);
	void SetInput(iInput* input);
	void SetSound(iSound* sound);

	iRender* SwapRender(iRender* render);
	iInput* SwapInput(iInput* input);
	iSound* SwapSound(iSound* sound);


	void CleanMemory();
	void CleanRegisters();
	void CleanStack();
	void CleanGfx();
private:
	Cpu _cpu;

};






inline size_t CpuManager::GetMemorySize() const { return utility::arr_size(_cpu.memory); }
inline size_t CpuManager::GetRegistersSize() const { return utility::arr_size(_cpu.registers); }
inline size_t CpuManager::GetStackSize() const { return utility::arr_size(_cpu.stack); }
inline size_t CpuManager::GetGfxSize() const { return utility::arr_size(_cpu.gfx); }
inline bool CpuManager::GetErrorFlag() const { return _cpu.errorFlag; }
inline const iRender* CpuManager::GetRender() const { return _cpu.render; }
inline const iInput* CpuManager::GetInput() const { return _cpu.input; }
inline const iSound* CpuManager::GetSound() const { return _cpu.sound; }
inline const uint8_t* CpuManager::GetMemory() const { return _cpu.memory; }
inline const uint8_t* CpuManager::GetRegisters() const { return _cpu.registers; }
inline const size_t* CpuManager::GetStack() const { return _cpu.stack; }
inline const uint32_t* CpuManager::GetGfx() const { return _cpu.gfx; }
inline const Cpu& CpuManager::GetCpu() const { return _cpu; }


inline Cpu& CpuManager::GetCpu() { return _cpu; }
inline iRender* CpuManager::GetRender() { return _cpu.render; }
inline iInput* CpuManager::GetInput() { return _cpu.input; }
inline iSound* CpuManager::GetSound() { return _cpu.sound; }
inline void CpuManager::SetErrorFlag(const bool val) { _cpu.errorFlag = val; }
inline void CpuManager::SetPC(const size_t offset) { _cpu.pc = offset; }
inline void CpuManager::SetSP(const size_t offset) { _cpu.sp = offset; }
inline void CpuManager::SetRender(iRender* render) { _cpu.render = render; }
inline void CpuManager::SetInput(iInput* input) { _cpu.input = input; }
inline void CpuManager::SetSound(iSound* sound) { _cpu.sound = sound; }


inline void CpuManager::CleanMemory() 
{ 
	utility::arr_zero(_cpu.memory); 
}


inline void CpuManager::CleanRegisters()
{
	utility::arr_zero(_cpu.registers);
	_cpu.I = 0;
	_cpu.delayTimer = 0;
	_cpu.soundTimer = 0;
}


inline void CpuManager::CleanStack()
{
	utility::arr_zero(_cpu.stack);
	_cpu.sp = 0;
}


inline void CpuManager::CleanGfx() 
{ 
	utility::arr_zero(_cpu.gfx); 
}









} // xchip namespace

#endif
