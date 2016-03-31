#include <XChip/Emulator.h>
#include <XChip/Fonts.h>
#include <XChip/Interfaces/iRender.h>
#include <XChip/Interfaces/iInput.h>
#include <XChip/Interfaces/iSound.h>
#include <XChip/Instructions.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Alloc.h>

namespace xchip {

Emulator::Emulator() noexcept 
	: _instrf(false),
	_drawf(false),
	_exitf(true),
	_initialized(false)
	
{
	using namespace utility::literals;
	_instrTimer.SetTargetTime(358_hz);
	_frameTimer.SetTargetTime(60_hz);
	utility::LOG("Creating Emulator...");
}


Emulator::~Emulator()
{
	if(_initialized) 
		this->Dispose();

	utility::LOG("Destroying Emulator...");
}


bool Emulator::Initialize(iRender* const render, iInput* const input, iSound* const sound) noexcept
{
	if (_initialized) 
		this->Dispose();

	// Default Chip8 Mode
	if (!_manager.SetMemory(0xFFFF)
		|| !_manager.SetRegisters(0x10)
		|| !_manager.SetStack(0x10)
		|| !_manager.SetGfx(64 * 32))
	{
		return false;
	}

	_manager.SetFont(fonts::chip8_default_font, 80);

	if (! this->InitRender(render) 
		|| ! this->InitInput(input)
		|| ! this->InitSound(sound))
	{
			this->Dispose();
			return false;
	}
	

	_exitf = false;
	_initialized = true;


	// little trick:
	// use the last  ( sizeof(bool*) + sizeof(uint8_t) ) bytes in _cpu's memory to store a pointer
	// to our exit flag and the 0xFF value to say that the pointer is available
	// ( normaly 5 bytes in x86 and 9 bytes in x64).
	Cpu& _cpu = _manager.GetCpu();
	std::size_t exitf_offs = utility::get_arr_size(static_cast<uint8_t*>(_cpu.memory)) - sizeof(bool*) - 1;
	_cpu.memory[exitf_offs - 1] = 0xFF;
	reinterpret_cast<bool*&>(_cpu.memory[exitf_offs]) = &_exitf;
	
	return true;
}



void Emulator::Dispose() noexcept
{
	if (_initialized)
	{
		auto rend = _manager.SwapRender(nullptr);
		auto input = _manager.SwapInput(nullptr);
		auto sound = _manager.SwapSound(nullptr);

		if (rend) 
			delete rend;
		if (input) 
			delete input;
		if (sound)
			delete sound;

		_manager.Dispose();
		_instrf = false;
		_drawf = false;
		_exitf = true;
		_initialized = false;
	}
}


void Emulator::HaltForNextFlag() const
{
	if (!_instrf && !_drawf)
	{
		const auto instrRemain = _instrTimer.GetRemain();
		const auto frameRemain = _frameTimer.GetRemain();
		utility::Timer::Halt((instrRemain < frameRemain) ? instrRemain : frameRemain);
	}
}



void Emulator::UpdateTimers()
{
	using namespace utility::literals;

	if (!_instrf && _instrTimer.Finished())
	{
		_instrf = true;
		_instrTimer.Start();
	}

	if (!_drawf && _frameTimer.Finished())
	{
		_drawf = true;
		_frameTimer.Start();
	}

	
	static utility::Timer chip8Timers( 48_hz );

	if (chip8Timers.Finished())
	{
		Cpu& _cpu = _manager.GetCpu();
		
		if (_cpu.delayTimer)
			--_cpu.delayTimer;

		if (_cpu.soundTimer) 
		{
			--_cpu.soundTimer;
		}
		else if (_cpu.sound->IsPlaying())
		{
			_cpu.sound->Stop();
		}

		chip8Timers.Start();
	}
}

void Emulator::UpdateSystems()
{
	_manager.GetRender()->UpdateEvents();
	_manager.GetInput()->UpdateKeys();
	this->UpdateTimers();
}


void Emulator::ExecuteInstr()
{
	Cpu& _cpu = _manager.GetCpu();
	_cpu.opcode = (_cpu.memory[_cpu.pc] << 8) | _cpu.memory[_cpu.pc+1];
	_cpu.pc += 2;
	instructions::instrTable[(_cpu.opcode & 0xf000) >> 12](&_cpu);
	_instrf = false;
}


void Emulator::Draw()
{
	_manager.GetRender()->DrawBuffer();
	_drawf = false;
}


void Emulator::Reset()
{
	_manager.CleanGfx();
	_manager.CleanStack();
	_manager.CleanRegisters();
	Cpu& _cpu = _manager.GetCpu();
	_cpu.pc = 0x200;
	_cpu.sp = 0;
	_cpu.delayTimer = 0;
	_cpu.soundTimer = 0;
}



void Emulator::SetInstrPerSec(const unsigned short value)
{
	_instrTimer.SetTargetTime(utility::literals::operator""_hz(value));
}

void Emulator::SetFramesPerSec(const unsigned short value)
{
	_frameTimer.SetTargetTime(utility::literals::operator""_hz(value));
}




bool Emulator::InitRender(iRender* const rend)
{
	if (!rend) 
		return false;
	else if (rend->IsInitialized()) 
		return true;
	else if (!rend->Initialize(64, 32)) 
		return false;

	rend->SetBuffer(_manager.GetGfx());
	rend->SetWinCloseCallback(&_exitf, [](const void* exitf) {*(bool*)exitf = true; });

	auto oldrend = _manager.SwapRender(rend);
	if (oldrend) 
		delete oldrend;

	return true;
}



bool Emulator::InitInput(iInput* const input)
{
	if (!input) 
		return false;
	else if (input->IsInitialized()) 
		return true;
	else if (!input->Initialize()) 
		return false;

	input->SetEscapeKeyCallback(&_exitf, [](const void* exitf) {*(bool*)exitf = true; });
	input->SetResetKeyCallback(this, [](const void* _this)
	{
		((Emulator*)_this)->Reset();
	});


	input->SetWaitKeyCallback(this, [](const void* emu)
	{
		auto emulator = (Emulator* const) emu;
		do
		{
			emulator->UpdateSystems();
			if (emulator->GetExitFlag()) 
				return false;

			emulator->HaltForNextFlag();
			if (emulator->GetDrawFlag())
				emulator->Draw();

		} while (! emulator->GetInstrFlag());

		return true;
	});

	auto oldinput = _manager.SwapInput(input);
	if (oldinput) 
		delete oldinput;

	return true;
}


bool Emulator::InitSound(iSound* const sound)
{
	if(!sound)
		return false;
	else if(sound->IsInitialized())
		return true;
	else if(!sound->Initialize())
		return false;

	auto oldsound = _manager.SwapSound(sound);
	if(oldsound)
		delete oldsound;

	return true;
}












}
