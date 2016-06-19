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

#ifndef _XCHIP_CPU_MANAGER_H_
#define _XCHIP_CPU_MANAGER_H_

#include <Xlib/Alloc.h>
#include <Xlib/Vector2.h>

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
	const xlib::Vec2i& GetGfxRes() const;


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
	const uint32_t& GetGfx(const xlib::Vec2i& point) const;
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
	uint32_t& GetGfx(const xlib::Vec2i& point);
	uint32_t& GetGfx(const int x, const int y);
	

	void FetchOpcode();
	bool SetMemory(const size_t size);
	bool SetRegisters(const size_t size);
	bool SetStack(const size_t size);
	bool SetGfxRes(const xlib::Vec2i& res);
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
	Cpu _cpu;
	xlib::Vec2i _gfxRes = {0, 0};
};







inline uint8_t CpuManager::GetDelayTimer() const { return _cpu.delayTimer; }
inline uint8_t CpuManager::GetSoundTimer() const { return _cpu.soundTimer; }
inline uint16_t CpuManager::GetOpcode() const { return _cpu.opcode; }
inline uint16_t CpuManager::GetOpcode(const uint16_t mask) const { return _cpu.opcode & mask; }
inline uint32_t CpuManager::GetFlags() const { return _cpu.flags; }
inline uint32_t CpuManager::GetFlags(const uint32_t flags) const { return _cpu.flags & flags; }
inline size_t CpuManager::GetIndexRegister() const { return _cpu.I; }
inline size_t CpuManager::GetPC() const { return _cpu.pc; }
inline size_t CpuManager::GetSP() const { return _cpu.sp; }
inline size_t CpuManager::GetMemorySize() const { return xlib::arr_size(_cpu.memory); }
inline size_t CpuManager::GetRegistersSize() const { return xlib::arr_size(_cpu.registers); }
inline size_t CpuManager::GetStackSize() const { return xlib::arr_size(_cpu.stack); }
inline size_t CpuManager::GetGfxSize() const { return xlib::arr_size(_cpu.gfx); }
inline const xlib::Vec2i& CpuManager::GetGfxRes() const { return _gfxRes; }

inline const iRender* CpuManager::GetRender() const { return _cpu.render; }
inline const iInput* CpuManager::GetInput() const { return _cpu.input; }
inline const iSound* CpuManager::GetSound() const { return _cpu.sound; }
inline const uint8_t* CpuManager::GetMemory() const { return _cpu.memory; }
inline const uint8_t* CpuManager::GetRegisters() const { return _cpu.registers; }
inline const size_t* CpuManager::GetStack() const { return _cpu.stack; }
inline const uint32_t* CpuManager::GetGfx() const { return _cpu.gfx; }
inline const Cpu& CpuManager::GetCpu() const { return _cpu; }



inline const uint8_t& CpuManager::GetMemory(const size_t offset) const 
{ 
	ASSERT_MSG(GetMemorySize() > offset, "memory overflow"); 
	return _cpu.memory[offset]; 
}

inline const uint8_t& CpuManager::GetRegisters(const size_t offset) const
{
	ASSERT_MSG(GetRegistersSize() > offset, "registers overflow"); 
	return _cpu.registers[offset]; 
}


inline const size_t& CpuManager::GetStack(const size_t offset) const 
{ 
	ASSERT_MSG(GetStackSize() > offset, "stack overflow");
	return _cpu.stack[offset]; 

}


inline const uint32_t& CpuManager::GetGfx(const size_t offset) const 
{ 
	ASSERT_MSG(GetGfxSize() > offset, "GFX overflow"); 
	return _cpu.gfx[offset]; 
}


inline const uint32_t& CpuManager::GetGfx(const xlib::Vec2i& point) const  
{
	ASSERT_MSG(_gfxRes.x >= point.x && _gfxRes.y >= point.y, "GFX overflow"); 
	return _cpu.gfx[ ( _gfxRes.x * point.y ) + point.x]; 
}



inline const uint32_t& CpuManager::GetGfx(const int x, const int y) const  
{
	ASSERT_MSG(_gfxRes.x >= x && _gfxRes.y >= y, "GFX overflow"); 
	return _cpu.gfx[ ( _gfxRes.x * y ) + x]; 
}






inline iRender* CpuManager::GetRender() { return _cpu.render; }
inline iInput* CpuManager::GetInput() { return _cpu.input; }
inline iSound* CpuManager::GetSound() { return _cpu.sound; }
inline uint8_t* CpuManager::GetMemory() { return _cpu.memory; }
inline uint8_t* CpuManager::GetRegisters() { return _cpu.registers; }
inline size_t* CpuManager::GetStack() { return _cpu.stack; }
inline uint32_t* CpuManager::GetGfx() { return _cpu.gfx; }
inline Cpu& CpuManager::GetCpu() { return _cpu; }


inline uint8_t& CpuManager::GetMemory(const size_t offset) 
{ 
	ASSERT_MSG(GetMemorySize() > offset, "memory overflow"); 
	return _cpu.memory[offset]; 
}


inline uint8_t& CpuManager::GetRegisters(const size_t offset) 
{ 
	ASSERT_MSG(GetRegistersSize() > offset, "registers overflow"); 
	return _cpu.registers[offset]; 
}
 
inline size_t& CpuManager::GetStack(const size_t offset)  
{ 
	ASSERT_MSG(GetStackSize() > offset, "stack overflow"); 
	return _cpu.stack[offset]; 
}


inline uint32_t& CpuManager::GetGfx(const size_t offset) 
{ 
	ASSERT_MSG(GetGfxSize() > offset, "GFX overflow"); 
	return _cpu.gfx[offset]; 
}


inline uint32_t& CpuManager::GetGfx(const xlib::Vec2i& point)
{
	ASSERT_MSG(_gfxRes.x >= point.x && _gfxRes.y >= point.y, "GFX overflow"); 
	return _cpu.gfx[ ( _gfxRes.x * point.y ) + point.x]; 
}


inline uint32_t& CpuManager::GetGfx(const int x, const int y)
{
	ASSERT_MSG(_gfxRes.x >= x && _gfxRes.y >= y, "GFX overflow"); 
	return _cpu.gfx[ ( _gfxRes.x * y ) + x]; 
}


inline void CpuManager::FetchOpcode()
{
	_cpu.opcode = _cpu.memory[_cpu.pc] << 8 | _cpu.memory[_cpu.pc+1];
	_cpu.pc += 2;
}


inline void CpuManager::SetFlags(const uint32_t flags) { _cpu.flags |= flags; }
inline void CpuManager::UnsetFlags(const uint32_t flags) { _cpu.flags ^= flags; }
inline void CpuManager::CleanFlags() { _cpu.flags = 0; }
inline void CpuManager::SetDelayTimer(const uint8_t val) { _cpu.delayTimer = val; }
inline void CpuManager::SetSoundTimer(const uint8_t val) { _cpu.soundTimer = val; }
inline void CpuManager::SetOpcode(const uint16_t val) { _cpu.opcode = val; }
inline void CpuManager::SetIndexRegister(const size_t index) { _cpu.I = index; }
inline void CpuManager::SetPC(const size_t offset) { _cpu.pc = offset; }
inline void CpuManager::SetSP(const size_t offset) { _cpu.sp = offset; }


inline void CpuManager::CleanMemory() 
{ 
	xlib::arr_zero(_cpu.memory); 
}

inline void CpuManager::CleanRegisters() 
{ 
	xlib::arr_zero(_cpu.registers);
	_cpu.I = 0;
	_cpu.delayTimer = 0;
	_cpu.soundTimer = 0;
}


inline void CpuManager::CleanStack()
{
	xlib::arr_zero(_cpu.stack);
	_cpu.sp = 0;
}


inline void CpuManager::CleanGfx() 
{ 
	xlib::arr_zero(_cpu.gfx); 
}



inline constexpr size_t CpuManager::GetDefaultFontIndex() { return 0; }
inline constexpr size_t CpuManager::GetHiResFontIndex() { return xlib::arr_size(fonts::chip8DefaultFont);  }





} // xchip namespace

#endif
