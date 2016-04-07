#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_
#include <cstddef>
#include "Cpu.h"



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

	const iRender* GetRender() const;
	const iInput* GetInput() const;
	const iSound* GetSound() const;
	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const uint16_t* GetStack() const;
	const uint32_t* GetGfx() const;
	const Cpu& GetCpu() const;

	Cpu& GetCpu();
	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();
	
	void SetRender(iRender* render);
	void SetInput(iInput* input);
	void SetSound(iSound* sound);

	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfx(const size_t size);
	void SetPC(const uint16_t offset);
	void SetSP(const uint16_t offset);
	void SetFont(const uint8_t* font, const size_t size);
	bool LoadRom(const char* file, const size_t at);
	
	void InsertAddress(const void* addr, const size_t offset);
	template<class T>
	void InsertValue(const T val, const size_t offset);
	
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




inline const iRender* CpuManager::GetRender() const { return _cpu.render; }
inline const iInput* CpuManager::GetInput() const { return _cpu.input; }
inline const iSound* CpuManager::GetSound() const { return _cpu.sound; }
inline const uint8_t* CpuManager::GetMemory() const { return _cpu.memory; }
inline const uint8_t* CpuManager::GetRegisters() const { return _cpu.registers; }
inline const uint16_t* CpuManager::GetStack() const { return _cpu.stack; }
inline const uint32_t* CpuManager::GetGfx() const { return _cpu.gfx; }
inline const Cpu& CpuManager::GetCpu() const { return _cpu; }

inline void CpuManager::SetPC(const uint16_t offset) { _cpu.pc = offset; }
inline void CpuManager::SetSP(const uint16_t offset) { _cpu.sp = offset; }
inline Cpu& CpuManager::GetCpu() { return _cpu; }
inline iRender* CpuManager::GetRender() { return _cpu.render; }
inline iInput* CpuManager::GetInput() { return _cpu.input; }
inline iSound* CpuManager::GetSound() { return _cpu.sound; }


inline void CpuManager::InsertAddress(const void* addr, const size_t offset) 
{ 
	reinterpret_cast<const void*&>(_cpu.memory[offset]) = addr; 
}


template<class T>
inline void CpuManager::InsertValue(const T val, const size_t offset)
{
	*reinterpret_cast<T*>(_cpu.memory+offset) = val;
}
















} // xchip namespace

#endif
