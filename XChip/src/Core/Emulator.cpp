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

#include <XChip/Core/Emulator.h>
#include <XChip/Utils/Log.h>
#include <XChip/Utils/ScopeExit.h>




namespace xchip {

using namespace xchip::utils;


static void init_emu_timers(Timer& instrTimer, Timer& frameTimer, Timer& chDelayTimer) noexcept;
static bool init_cpu_manager(CpuManager& _manager) noexcept;






Emulator::Emulator() noexcept
{
	Log("Creating XChip Emulator object...");
}



Emulator::~Emulator()
{
	if(_initialized) 
		this->Dispose();

	Log("Destroying XChip Emulator object...");
}





bool Emulator::Initialize() noexcept
{
	if (_initialized) 
		this->Dispose();

	const auto scope = make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized)
			this->Dispose();
	});

	init_emu_timers(_instrTimer, _frameTimer, _chDelayTimer);

	if(init_cpu_manager(_manager))
	{
		CleanFlags();
		_initialized = true;
		return true;
	}

	return false;
}




bool Emulator::Initialize(UniqueRender&& render, UniqueInput&& input, UniqueSound&& sound) noexcept
{
	using std::move;

	if (_initialized) 
		this->Dispose();

	const auto scope = make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized)
			this->Dispose();
	});

	_renderPlugin = move(render);
	_inputPlugin = move(input);
	_soundPlugin = move(sound);

	init_emu_timers(_instrTimer, _frameTimer, _chDelayTimer);

	if(init_cpu_manager(_manager))
	{
		// try to init all interfaces before returning something...
		if (( InitRender() & InitInput() & InitSound()) ) 
		{
			CleanFlags();
			_initialized = true;
			return true;
		}
	}
	
	return false;
}





void Emulator::Dispose() noexcept
{
	_manager.Dispose();
	_initialized = false;
}





void Emulator::HaltForNextFlag() const
{
	if (! _manager.GetFlags(Cpu::DRAW | Cpu::INSTR))
	{
		const auto instrRemain = _instrTimer.GetRemain();
		const auto frameRemain = _frameTimer.GetRemain();
		Timer::Halt((instrRemain < frameRemain) ? instrRemain : frameRemain);
	}
}



void Emulator::UpdateTimers()
{
	if (!_manager.GetFlags(Cpu::INSTR) && _instrTimer.Finished())
	{
		_manager.SetFlags(Cpu::INSTR);
		_instrTimer.Start();
	}

	if (!_manager.GetFlags(Cpu::DRAW) && _frameTimer.Finished())
	{
		_manager.SetFlags(Cpu::DRAW);
		_frameTimer.Start();
	}

	

	if (_chDelayTimer.Finished())
	{
		auto& delayTimer = _manager.GetCpu().delayTimer;
		if (delayTimer)
			--delayTimer;

		_chDelayTimer.Start();
	}
}


 
void Emulator::UpdateSystems()
{
	ASSERT_MSG(!_manager.GetFlags(Cpu::BAD_RENDER), "BAD RENDER");
	ASSERT_MSG(!_manager.GetFlags(Cpu::BAD_INPUT),  "BAD INPUT");
	_manager.GetRender()->UpdateEvents();
	_manager.GetInput()->UpdateKeys();
	this->UpdateTimers();
}




void Emulator::CleanFlags()
{
	// clean flags but keep bad flags.
	const auto badFlags = _manager.GetFlags(Cpu::BAD_RENDER | Cpu::BAD_INPUT | Cpu::BAD_SOUND);
	_manager.CleanFlags();
	_manager.SetFlags(badFlags);
}



void Emulator::Reset()
{
	if(!_manager.GetFlags(Cpu::BAD_SOUND))
		_soundPlugin->Stop();

	CleanFlags();
	_manager.CleanGfx();
	_manager.CleanStack();
	_manager.CleanRegisters();
	_manager.SetPC(0x200);
}





bool Emulator::SetRender(UniqueRender rend) 
{ 
	Log("Setting new iRender...");
	_renderPlugin = std::move(rend);
	return InitRender();
}




bool Emulator::SetInput(UniqueInput input) 
{
	Log("Setting new iInput...");
	_inputPlugin = std::move(input);
	return InitInput();
}




bool Emulator::SetSound(UniqueSound sound) 
{
	Log("Setting new iSound...");
	_soundPlugin = std::move(sound);
	return InitSound();
}




UniqueRender Emulator::SwapRender(UniqueRender rend) 
{ 
	if (rend.get()) 
	{
		Log("Swapping iRender...");
		_renderPlugin.Swap(rend);
		InitRender();
		return rend;
	}

	Log("Swapping iRender to nullptr...");
	_manager.SetRender(nullptr);
	return std::move(_renderPlugin);
}




UniqueInput Emulator::SwapInput(UniqueInput input) 
{ 
	if (input.get())
	{
		Log("Swapping iInput...");
		_inputPlugin.Swap(input);
		InitInput();
		return input;
	}

	Log("Swapping iInput to nullptr...");
	_manager.SetInput(nullptr);
	return std::move(_inputPlugin);
}




UniqueSound Emulator::SwapSound(UniqueSound sound) 
{ 
	if (sound.get())
	{
		Log("Swapping iSound...");
		_soundPlugin.Swap(sound);
		InitSound();
		return sound;
	}

	Log("Swapping iSound to nullptr...");
	_manager.SetSound(nullptr);
	return std::move(_soundPlugin);
}










bool Emulator::InitRender()
{
	iRender* const rend = _renderPlugin.get();
	const auto end_scope = make_scope_exit([this]()noexcept 
	{ 
		_manager.SetRender(_renderPlugin.get());
	});


	if (!rend) {
		LogError("Cannot Initialize iRender: nullptr");
		return false;
	} 
	else if (rend->IsInitialized()) {
		return true;
	} 
	else if (!rend->Initialize({512, 256}, _manager.GetGfxRes())) {
		return false;
	}

	rend->SetBuffer(_manager.GetGfx());
	rend->SetWinCloseCallback(&_manager, [](const void* man) { ((CpuManager*)man)->SetFlags(Cpu::EXIT); });
	return true;
}




bool Emulator::InitInput()
{
	iInput* const input = _inputPlugin.get();
	const auto end_scope = make_scope_exit([this]()noexcept
	{
		_manager.SetInput(_inputPlugin.get());
	});


	if (!input) {
		LogError("Cannot Initialize iInput: nullptr");
		return false;
	}
	else if (input->IsInitialized()) {
		return true;
	}
	else if (!input->Initialize()) {
		return false;
	}


	input->SetEscapeKeyCallback(&_manager, [](const void* man){ ((CpuManager*)man)->SetFlags(Cpu::EXIT); });
	input->SetResetKeyCallback(this, [](const void* _this) { ((Emulator*)_this)->Reset(); });
	input->SetWaitKeyCallback(this, [](const void* g_emulator)
	{
		auto* const emulator = (Emulator*) g_emulator;
		
		do
		{		
			emulator->GetRender()->UpdateEvents();
			emulator->UpdateTimers();
			
			if (emulator->GetExitFlag())
				return false;

			if (emulator->GetDrawFlag())
				emulator->Draw();


			emulator->HaltForNextFlag();

		}while(!emulator->GetInstrFlag());


		return true;
	});

	return true;
}





bool Emulator::InitSound()
{
	iSound* const sound = _soundPlugin.get();
	const auto end_scope = make_scope_exit([this]()noexcept
	{
		_manager.SetSound(_soundPlugin.get());
	});

	if (!sound)  {
		LogError("Cannot Initialize iSound: nullptr");
		return false;
	}
	else if (sound->IsInitialized()) {
		return true;
	}
	else if (!sound->Initialize()) {
		return false;
	}

	sound->SetCountdownFreq(60);
	return true;
}











static void init_emu_timers(Timer& instrTimer, Timer& frameTimer, Timer& chDelayTimer) noexcept
{	
	using namespace xchip::utils::literals;

	instrTimer.SetTargetTime(380_hz);
	frameTimer.SetTargetTime(60_hz);
	chDelayTimer.SetTargetTime(60_hz);
}



static bool init_cpu_manager(CpuManager& manager) noexcept
{
	// init the CPU
	if (manager.SetMemory(0xFFFF)
		&& manager.SetRegisters(0x10)
		&& manager.SetStack(0x10)
		&& manager.SetGfxRes(64, 32))
	{
		manager.SetPC(0x200);
		manager.LoadDefaultFont();
		manager.LoadHiResFont();
		return true;
	}

	return false;
}










}

