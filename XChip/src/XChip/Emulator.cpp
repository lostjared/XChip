#include <XChip/Emulator.h>
#include <XChip/Fonts.h>
#include <XChip/Utility/Log.h>
#include <XChip/Interfaces/iRender.h>
#include <XChip/Interfaces/iInput.h>
#include <XChip/Instructions.h>

namespace xchip {



Emulator::Emulator() noexcept
	: _instrf(false),
	_drawf(false),
	_exitf(true),
	_initialized(false)
	
{
	using namespace utility;
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


bool Emulator::Initialize(iRender *render, iInput *input) noexcept
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

	

	if (!this->InitRender(render) || ! this->InitInput(input))
			return false;
	

	_exitf = false;
	_initialized = true;
	return true;
}



void Emulator::Dispose() noexcept
{
	if (_initialized)
	{
		auto rend = _manager.SwapRender(nullptr);
		auto input = _manager.SwapInput(nullptr);
		if (rend) delete rend;
		if (input) delete input;
		_manager.Dispose();
		_instrf = false;
		_drawf = false;
		_exitf = true;
		_initialized = false;
	}
}


void Emulator::HaltForNextFlag() const
{
	using namespace utility;
	if (!_instrf && !_drawf)
	{
		auto instrRemain = _instrTimer.GetRemain();
		auto frameRemain = _frameTimer.GetRemain();
		Timer::Halt((instrRemain < frameRemain) ? instrRemain : frameRemain);
	}
}



void Emulator::UpdateTimers()
{
	using namespace utility;

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

	static Timer chip8Timers{ 60_hz };

	if (chip8Timers.Finished())
	{
		if(_manager.GetCpu().delayTimer)
			--_manager.GetCpu().delayTimer;

		if (_manager.GetCpu().soundTimer)
		{
			// play beep...
			// temporary
			LOG("\a");
			--_manager.GetCpu().soundTimer;
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
	auto& _cpu = _manager.GetCpu();
	_cpu.opcode = (_cpu.memory[_cpu.pc] << 8) | _cpu.memory[_cpu.pc + 1];
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
	_manager.GetCpu().pc = 0x200;
	_manager.GetCpu().sp = 0;
	_manager.GetCpu().delayTimer = 0;
	_manager.GetCpu().soundTimer = 0;
}



void Emulator::SetInstrPerSec(unsigned short value)
{
	using namespace utility;
	_instrTimer.SetTargetTime(1_sec / value);
}

void Emulator::SetFramesPerSec(unsigned short value)
{
	using namespace utility;
	_frameTimer.SetTargetTime(1_sec / value);
}




bool Emulator::InitRender(iRender* rend)
{
	if (!rend) return false;
	else if (!rend->Initialize(64, 32)) return false;

	rend->SetBuffer(_manager.GetGfx());
	rend->SetWinCloseCallback(&_exitf, [](const void*exitf) {*(bool*)exitf = true; });
	rend->SetWinResizeCallback(nullptr, [](const void*) {utility::LOG("Window Resized"); });
	auto oldrend = _manager.SwapRender(rend);
	if (oldrend) delete oldrend;

	return true;
}



bool Emulator::InitInput(iInput* input)
{
	if (!input) return false;
	else if (!input->Initialize()) return false;

	input->SetEscapeKeyCallback(&_exitf, [](const void*exitf) {*(bool*)exitf = true; });
	input->SetResetKeyCallback(this, [](const void* _this)
	{
		((Emulator*)_this)->Reset();
	});

	input->SetWaitKeyCallback(this, [](const void* emu)
	{
		auto emulator = (Emulator*)emu;
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
	if (oldinput) delete oldinput;

	return true;
}















}
