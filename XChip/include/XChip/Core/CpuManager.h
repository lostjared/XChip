/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#ifndef XCHIP_CORE_MANAGER_H_
#define XCHIP_CORE_MANAGER_H_

#include <Utix/Alloc.h>
#include <Utix/Vector2.h>

#include "Cpu.h"
#include "Fonts.h"



namespace xchip {

class CpuManager
{
public:
	CpuManager() noexcept;
	~CpuManager();
	CpuManager(const CpuManager&) = delete;
	CpuManager& operator=(const CpuManager&) = delete;

	void Dispose() noexcept;


	uint8_t GetDelayTimer() const;
	uint8_t GetSoundTimer() const;
	uint16_t GetOpcode() const;
	uint16_t GetOpcode(const uint16_t mask) const;
	uint32_t GetFlags() const;
	uint32_t GetFlags(const uint32_t flags) const;
	size_t GetIndexRegister() const;
	size_t GetPC() const;
	size_t GetSP() const;
	size_t GetMemorySize() const;
	size_t GetRegistersSize() const;
	size_t GetStackSize() const;
	size_t GetGfxSize() const;
	const utix::Vec2i& GetGfxRes() const;


	const iRender* GetRender() const;
	const iInput* GetInput() const;
	const iSound* GetSound() const;
	const uint8_t* GetMemory() const;
	const uint8_t* GetRegisters() const;
	const size_t* GetStack() const;
	const uint32_t* GetGfx() const;
	const Cpu& GetCpu() const;
	const uint8_t& GetMemory(const size_t offset) const;
	const uint8_t& GetRegisters(const size_t offset) const;
	const size_t& GetStack(const size_t offset) const;
	const uint32_t& GetGfx(const size_t offset) const;
	const uint32_t& GetGfx(const utix::Vec2i& point) const;
	const uint32_t& GetGfx(const int x, const int y) const;


	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();
	uint8_t* GetMemory();
	uint8_t* GetRegisters();
	size_t* GetStack();
	uint32_t* GetGfx();
	Cpu& GetCpu();
	uint8_t& GetMemory(const size_t offset);
	uint8_t& GetRegisters(const size_t offset);
	size_t& GetStack(const size_t offset);
	uint32_t& GetGfx(const size_t offset);
	uint32_t& GetGfx(const utix::Vec2i& point);
	uint32_t& GetGfx(const int x, const int y);
	

	void FetchOpcode();
	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfxRes(const utix::Vec2i& res);
	bool SetGfxRes(const int w, const int h);
	bool ResizeMemory(const size_t size);
	bool ResizeRegisters(const size_t size);
	bool ResizeStack(const size_t size);
	void SetFlags(const uint32_t flags);
	void UnsetFlags(const uint32_t flags);
	void CleanFlags();
	void SetDelayTimer(const uint8_t val);
	void SetSoundTimer(const uint8_t val);
	void SetOpcode(const uint16_t val);
	void SetIndexRegister(const size_t index);
	void SetPC(const size_t offset);
	void SetSP(const size_t offset);
	void LoadDefaultFont();
	void LoadHiResFont();
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

	static constexpr size_t GetDefaultFontIndex();
	static constexpr size_t GetHiResFontIndex();

private:
	Cpu m_cpu;
	utix::Vec2i m_gfxRes = {0, 0};
};







inline uint8_t CpuManager::GetDelayTimer() const { return m_cpu.delayTimer; }
inline uint8_t CpuManager::GetSoundTimer() const { return m_cpu.soundTimer; }
inline uint16_t CpuManager::GetOpcode() const { return m_cpu.opcode; }
inline uint16_t CpuManager::GetOpcode(const uint16_t mask) const { return m_cpu.opcode & mask; }
inline uint32_t CpuManager::GetFlags() const { return m_cpu.flags; }
inline uint32_t CpuManager::GetFlags(const uint32_t flags) const { return m_cpu.flags & flags; }
inline size_t CpuManager::GetIndexRegister() const { return m_cpu.I; }
inline size_t CpuManager::GetPC() const { return m_cpu.pc; }
inline size_t CpuManager::GetSP() const { return m_cpu.sp; }
inline size_t CpuManager::GetMemorySize() const { return utix::arr_size(m_cpu.memory); }
inline size_t CpuManager::GetRegistersSize() const { return utix::arr_size(m_cpu.registers); }
inline size_t CpuManager::GetStackSize() const { return utix::arr_size(m_cpu.stack); }
inline size_t CpuManager::GetGfxSize() const { return utix::arr_size(m_cpu.gfx); }
inline const utix::Vec2i& CpuManager::GetGfxRes() const { return m_gfxRes; }

inline const iRender* CpuManager::GetRender() const { return m_cpu.render; }
inline const iInput* CpuManager::GetInput() const { return m_cpu.input; }
inline const iSound* CpuManager::GetSound() const { return m_cpu.sound; }
inline const uint8_t* CpuManager::GetMemory() const { return m_cpu.memory; }
inline const uint8_t* CpuManager::GetRegisters() const { return m_cpu.registers; }
inline const size_t* CpuManager::GetStack() const { return m_cpu.stack; }
inline const uint32_t* CpuManager::GetGfx() const { return m_cpu.gfx; }
inline const Cpu& CpuManager::GetCpu() const { return m_cpu; }



inline const uint8_t& CpuManager::GetMemory(const size_t offset) const 
{ 
	ASSERT_MSG(GetMemorySize() > offset, "memory overflow"); 
	return m_cpu.memory[offset]; 
}

inline const uint8_t& CpuManager::GetRegisters(const size_t offset) const
{
	ASSERT_MSG(GetRegistersSize() > offset, "registers overflow"); 
	return m_cpu.registers[offset]; 
}


inline const size_t& CpuManager::GetStack(const size_t offset) const 
{ 
	ASSERT_MSG(GetStackSize() > offset, "stack overflow");
	return m_cpu.stack[offset]; 

}


inline const uint32_t& CpuManager::GetGfx(const size_t offset) const 
{ 
	ASSERT_MSG(GetGfxSize() > offset, "GFX overflow"); 
	return m_cpu.gfx[offset]; 
}


inline const uint32_t& CpuManager::GetGfx(const utix::Vec2i& point) const  
{
	ASSERT_MSG(m_gfxRes.x >= point.x && m_gfxRes.y >= point.y, "GFX overflow"); 
	return m_cpu.gfx[ ( m_gfxRes.x * point.y ) + point.x]; 
}



inline const uint32_t& CpuManager::GetGfx(const int x, const int y) const  
{
	ASSERT_MSG(m_gfxRes.x >= x && m_gfxRes.y >= y, "GFX overflow"); 
	return m_cpu.gfx[ ( m_gfxRes.x * y ) + x]; 
}






inline iRender* CpuManager::GetRender() { return m_cpu.render; }
inline iInput* CpuManager::GetInput() { return m_cpu.input; }
inline iSound* CpuManager::GetSound() { return m_cpu.sound; }
inline uint8_t* CpuManager::GetMemory() { return m_cpu.memory; }
inline uint8_t* CpuManager::GetRegisters() { return m_cpu.registers; }
inline size_t* CpuManager::GetStack() { return m_cpu.stack; }
inline uint32_t* CpuManager::GetGfx() { return m_cpu.gfx; }
inline Cpu& CpuManager::GetCpu() { return m_cpu; }


inline uint8_t& CpuManager::GetMemory(const size_t offset) 
{ 
	ASSERT_MSG(GetMemorySize() > offset, "memory overflow"); 
	return m_cpu.memory[offset]; 
}


inline uint8_t& CpuManager::GetRegisters(const size_t offset) 
{ 
	ASSERT_MSG(GetRegistersSize() > offset, "registers overflow"); 
	return m_cpu.registers[offset]; 
}
 
inline size_t& CpuManager::GetStack(const size_t offset)  
{ 
	ASSERT_MSG(GetStackSize() > offset, "stack overflow"); 
	return m_cpu.stack[offset]; 
}


inline uint32_t& CpuManager::GetGfx(const size_t offset) 
{ 
	ASSERT_MSG(GetGfxSize() > offset, "GFX overflow"); 
	return m_cpu.gfx[offset]; 
}


inline uint32_t& CpuManager::GetGfx(const utix::Vec2i& point)
{
	ASSERT_MSG(m_gfxRes.x >= point.x && m_gfxRes.y >= point.y, "GFX overflow"); 
	return m_cpu.gfx[ ( m_gfxRes.x * point.y ) + point.x]; 
}


inline uint32_t& CpuManager::GetGfx(const int x, const int y)
{
	ASSERT_MSG(m_gfxRes.x >= x && m_gfxRes.y >= y, "GFX overflow"); 
	return m_cpu.gfx[ ( m_gfxRes.x * y ) + x]; 
}


inline void CpuManager::FetchOpcode()
{
	m_cpu.opcode = m_cpu.memory[m_cpu.pc] << 8 | m_cpu.memory[m_cpu.pc+1];
	m_cpu.pc += 2;
}


inline void CpuManager::SetFlags(const uint32_t flags) { m_cpu.flags |= flags; }
inline void CpuManager::UnsetFlags(const uint32_t flags) { m_cpu.flags &= ~flags; }
inline void CpuManager::CleanFlags() { m_cpu.flags = 0; }
inline void CpuManager::SetDelayTimer(const uint8_t val) { m_cpu.delayTimer = val; }
inline void CpuManager::SetSoundTimer(const uint8_t val) { m_cpu.soundTimer = val; }
inline void CpuManager::SetOpcode(const uint16_t val) { m_cpu.opcode = val; }
inline void CpuManager::SetIndexRegister(const size_t index) { m_cpu.I = index; }
inline void CpuManager::SetPC(const size_t offset) { m_cpu.pc = offset; }
inline void CpuManager::SetSP(const size_t offset) { m_cpu.sp = offset; }


inline void CpuManager::CleanMemory() 
{ 
	utix::arr_zero(m_cpu.memory); 
}

inline void CpuManager::CleanRegisters() 
{ 
	utix::arr_zero(m_cpu.registers);
	m_cpu.I = 0;
	m_cpu.delayTimer = 0;
	m_cpu.soundTimer = 0;
}


inline void CpuManager::CleanStack()
{
	utix::arr_zero(m_cpu.stack);
	m_cpu.sp = 0;
}


inline void CpuManager::CleanGfx() 
{ 
	utix::arr_zero(m_cpu.gfx); 
}



constexpr size_t CpuManager::GetDefaultFontIndex() { return 0; }
constexpr size_t CpuManager::GetHiResFontIndex() { return utix::arr_size(fonts::chip8DefaultFont);  }





} 










#endif // XCHIP_CPU_MANAGER_H_
