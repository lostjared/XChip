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

#ifndef _XCHIP_EMULATOR_H_
#define _XCHIP_EMULATOR_H_
#include <string>

#include "CpuManager.h"
#include "Instructions.h"
#include <XChip/Plugins.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Utility/Assert.h>

 

namespace xchip {
using UniqueRender = UniquePlugin<iRender>;
using UniqueInput = UniquePlugin<iInput>;
using UniqueSound = UniquePlugin<iSound>;





class Emulator
{
public:
	Emulator() noexcept;
	~Emulator();
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	bool Initialize() noexcept;
	bool Initialize(UniqueRender&& render, UniqueInput&& input, UniqueSound&& sound) noexcept;

	void Dispose() noexcept;
	bool IsInitialized() const;
	bool Good() const;
	bool GetInstrFlag() const;
	bool GetDrawFlag() const;
	bool GetExitFlag() const;
	void HaltForNextFlag() const;
	int GetCpuFreq() const;
	int GetFps() const;
	const iRender* GetRender() const;
	const iInput* GetInput() const;
	const iSound* GetSound() const;

	void UpdateSystems();
	void ExecuteInstr();
	void CleanFlags();
	void Draw();
	void Reset();

	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();

	void SetDrawFlag(const bool val);
	void SetExitFlag(const bool val);
	void SetCpuFreq(const int value);
	void SetFps(const int value);
	bool LoadRom(const std::string& fileName);
	bool SetRender(UniqueRender rend);
	bool SetInput(UniqueInput input);
	bool SetSound(UniqueSound sound);
	UniqueRender SwapRender(UniqueRender rend = nullptr);
	UniqueInput SwapInput(UniqueInput input = nullptr);
	UniqueSound SwapSound(UniqueSound sound = nullptr);
	template<class P>
	bool SetPlugin(P&&);
	template<class P>
	P SwapPlugin(P&&);

private:
 	void UpdateTimers();
	bool InitRender();
	bool InitInput();
	bool InitSound();

	CpuManager _manager;
	utility::Timer _instrTimer;
	utility::Timer _frameTimer;
	utility::Timer _chDelayTimer;
	UniqueRender _renderPlugin;
	UniqueInput _inputPlugin;
	UniqueSound _soundPlugin;
	bool _initialized = false;
};





inline bool Emulator::IsInitialized() const { return _initialized; }
inline bool Emulator::Good() const { return _manager.GetFlags(Cpu::BAD_RENDER | Cpu::BAD_INPUT | Cpu::BAD_SOUND) == 0u; }
inline bool Emulator::GetInstrFlag() const { return _manager.GetFlags(Cpu::INSTR) != 0u; }
inline bool Emulator::GetDrawFlag() const { return _manager.GetFlags(Cpu::DRAW) != 0u; }
inline bool Emulator::GetExitFlag() const { return _manager.GetFlags(Cpu::EXIT) != 0u; }
inline const iRender* Emulator::GetRender() const { return _manager.GetRender(); }
inline const iInput* Emulator::GetInput() const { return _manager.GetInput(); }
inline const iSound* Emulator::GetSound() const { return _manager.GetSound(); }

inline int Emulator::GetCpuFreq() const 
{
	using namespace utility::literals;
	return static_cast<int>(1_sec / _instrTimer.GetTarget());
}

inline int Emulator::GetFps() const 
{
	using namespace utility::literals;
	return static_cast<int>(1_sec / _frameTimer.GetTarget());
}



inline void Emulator::SetCpuFreq(const int value)
{
	using namespace utility::literals;
	_instrTimer.SetTargetTime(operator""_hz(value > 0 ? value : 1));
}

inline void Emulator::SetFps(const int value)
{
	using namespace utility::literals;
	_frameTimer.SetTargetTime(operator""_hz(value > 0 ? value : 1));
}


inline void Emulator::SetDrawFlag(const bool val) 
{ 
	if (val)
		_manager.SetFlags(Cpu::DRAW);
	else
		_manager.UnsetFlags(Cpu::DRAW);
}


inline void Emulator::SetExitFlag(const bool val) 
{
	if (val)
		_manager.SetFlags(Cpu::EXIT);
	else
		_manager.UnsetFlags(Cpu::EXIT);
}



inline bool Emulator::LoadRom(const std::string& fname) { return _manager.LoadRom(fname.c_str(), 0x200); }

inline iRender* Emulator::GetRender() { return _manager.GetRender(); }
inline iInput* Emulator::GetInput() { return _manager.GetInput(); }
inline iSound* Emulator::GetSound() { return _manager.GetSound(); }

inline void Emulator::ExecuteInstr()
{
	instructions::execute_instruction(_manager);
	_manager.UnsetFlags(Cpu::INSTR);
}


inline void Emulator::Draw()
{
	ASSERT_MSG( !_manager.GetFlags(Cpu::BAD_RENDER), "bad render!");
	_manager.GetRender()->DrawBuffer();
	_manager.UnsetFlags(Cpu::DRAW);
}


template<>
inline bool Emulator::SetPlugin<UniqueRender>(UniqueRender&& plugin) { return this->SetRender(std::move(plugin)); }
template<>
inline bool Emulator::SetPlugin<UniqueInput>(UniqueInput&& plugin) { return this->SetInput(std::move(plugin)); }
template<>
inline bool Emulator::SetPlugin<UniqueSound>(UniqueSound&& plugin) { return this->SetSound(std::move(plugin)); }

template<>
inline UniqueRender Emulator::SwapPlugin<UniqueRender>(UniqueRender&& plugin) { return this->SwapRender(std::move(plugin)); }
template<>
inline UniqueInput Emulator::SwapPlugin<UniqueInput>(UniqueInput&& plugin) { return this->SwapInput(std::move(plugin)); }
template<>
inline UniqueSound Emulator::SwapPlugin<UniqueSound>(UniqueSound&& plugin) { return this->SwapSound(std::move(plugin)); }





}
#endif
