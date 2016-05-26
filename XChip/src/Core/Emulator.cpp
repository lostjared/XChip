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
#include <XChip/Core/Fonts.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>




namespace xchip {
using utility::literals::operator""_hz;

static bool init_cpu_manager(CpuManager& _manager) noexcept;



Emulator::Emulator() noexcept
	: _instrTimer(358_hz),
	_frameTimer(60_hz),
	_chDelayTimer(60_hz)
{
	utility::LOG("Creating Emulator object...");
}



Emulator::~Emulator()
{
	if(_initialized) 
		this->Dispose();

	utility::LOG("Destroying Emulator object...");
}









bool Emulator::Initialize() noexcept
{

	if (_initialized) 
		this->Dispose();

	const auto scope = utility::make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized)
			this->Dispose();
	});


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

	const auto scope = utility::make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized)
			this->Dispose();
	});

	_renderPlugin = move(render);
	_inputPlugin = move(input);
	_soundPlugin = move(sound);

	if(!init_cpu_manager(_manager))
		return false;

	// try to init all interfaces before returning something...
	if (!( InitRender() & InitInput() & InitSound()) ) 
		return false;

	CleanFlags();
	_initialized = true;
	return true;
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
		utility::Timer::Halt((instrRemain < frameRemain) ? instrRemain : frameRemain);
	}
}



void Emulator::UpdateTimers()
{
	using namespace utility::literals;

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
	const auto badFlags = _manager.GetFlags(Cpu::BAD_RENDER | Cpu::BAD_INPUT | Cpu::BAD_SOUND);
	_manager.CleanFlags();
	_manager.SetFlags(badFlags);
}



void Emulator::Reset()
{
	ASSERT_MSG(!_manager.GetFlags(Cpu::BAD_SOUND), "BAD SOUND");

	if(_manager.GetSound()->IsPlaying())
		_manager.GetSound()->Stop();

	
	CleanFlags();
	_manager.CleanGfx();
	_manager.CleanStack();
	_manager.CleanRegisters();
	_manager.SetPC(0x200);
}





bool Emulator::SetRender(UniqueRender rend) 
{ 
	utility::LOG("Setting new iRender...");
	_renderPlugin = std::move(rend);
	return InitRender();
}




bool Emulator::SetInput(UniqueInput input) 
{
	utility::LOG("Setting new iInput...");
	_inputPlugin = std::move(input);
	return InitInput();
}




bool Emulator::SetSound(UniqueSound sound) 
{
	utility::LOG("Setting new iSound...");
	_soundPlugin = std::move(sound);
	return InitSound();
}




UniqueRender Emulator::SwapRender(UniqueRender rend) 
{ 
	if (rend.get()) 
	{
		utility::LOG("Swapping iRender...");
		_renderPlugin.Swap(rend);
		InitRender();
		return rend;
	}

	utility::LOG("Swapping iRender to nullptr...");
	_manager.SetRender(nullptr);
	return std::move(_renderPlugin);
}




UniqueInput Emulator::SwapInput(UniqueInput input) 
{ 
	if (input.get())
	{
		utility::LOG("Swapping iInput...");
		_inputPlugin.Swap(input);
		InitInput();
		return input;
	}

	utility::LOG("Swapping iInput to nullptr...");
	_manager.SetInput(nullptr);
	return std::move(_inputPlugin);
}




UniqueSound Emulator::SwapSound(UniqueSound sound) 
{ 
	if (sound.get())
	{
		utility::LOG("Swapping iSound...");
		_soundPlugin.Swap(sound);
		InitSound();
		return sound;
	}

	utility::LOG("Swapping iSound to nullptr...");
	_manager.SetSound(nullptr);
	return std::move(_soundPlugin);
}










bool Emulator::InitRender()
{
	iRender* const rend = _renderPlugin.get();
	const auto end_scope = utility::make_scope_exit([this]()noexcept 
	{ 
		_manager.SetRender(_renderPlugin.get());
	});


	if (!rend) {
		utility::LOGerr("Cannot Initialize iRender: nullptr");
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
	const auto end_scope = utility::make_scope_exit([this]()noexcept
	{
		_manager.SetInput(_inputPlugin.get());
	});


	if (!input) {
		utility::LOGerr("Cannot Initialize iInput: nullptr");
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
			emulator->UpdateSystems();

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
	const auto end_scope = utility::make_scope_exit([this]()noexcept
	{
		_manager.SetSound(_soundPlugin.get());
	});

	if (!sound)  {
		utility::LOGerr("Cannot Initialize iSound: nullptr");
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











static bool init_cpu_manager(CpuManager& manager) noexcept
{

	using fonts::chip8DefaultFont;
	using fonts::chip8HiResFont;
	using utility::arr_size;

	// init the CPU
	if (manager.SetMemory(0xFFFF)
		&& manager.SetRegisters(0x10)
		&& manager.SetStack(0x10)
		&& manager.SetGfxRes(64, 32))
	{
		manager.SetPC(0x200);
		manager.LoadFont(chip8DefaultFont, arr_size(chip8DefaultFont), 0);
		manager.LoadFont(chip8HiResFont, arr_size(chip8HiResFont), arr_size(chip8DefaultFont));
		return true;
	}

	return false;
}










}

