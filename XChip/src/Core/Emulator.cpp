#include <XChip/Core/Emulator.h>
#include <XChip/Core/Fonts.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>




namespace xchip {
using utility::literals::operator""_hz;





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





bool Emulator::Initialize(UniqueRender render, UniqueInput input, UniqueSound sound) noexcept
{
	using fonts::chip8DefaultFont;
	using fonts::chip8HiResFont;
	using utility::arr_size;

	if (_initialized) 
		this->Dispose();

	

	const auto scope = utility::make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized)
			this->Dispose();
	});


	// Default Chip8 Mode
	if (!_manager.SetMemory(0xFFFF)
		|| !_manager.SetRegisters(0x10)
		|| !_manager.SetStack(0x10)
		|| !_manager.SetGfxRes({64, 32}))
	{
		return false;
	}

	_manager.SetPC(0x200);
	_manager.LoadFont(chip8DefaultFont, arr_size(chip8DefaultFont), 0);
	_manager.LoadFont(chip8HiResFont, arr_size(chip8HiResFont), arr_size(chip8DefaultFont));
	_manager.SetRender(render.release());
	_manager.SetInput(input.release());
	_manager.SetSound(sound.release());
	
	// try to init all interfaces before returning something...
	if (!(InitRender() & InitInput() & InitSound())) {
		return false;
	}

	CleanFlags();
	_initialized = true;
	return true;
}





void Emulator::Dispose() noexcept
{
	Cpu& chip = _manager.GetCpu();

	if (chip.render) {
		delete chip.render;
		chip.render = nullptr;
	}
	if (chip.input) {
		delete chip.input;
		chip.input = nullptr;
	}
	if (chip.sound) {
		delete chip.sound;
		chip.sound = nullptr;
	}
	
	_manager.Dispose();
	_instrf = false;
	_drawf = false;
	_exitf = true;
	_initialized = false;
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
	ASSERT_MSG(_manager.GetRender() != nullptr, "null render");
	ASSERT_MSG(_manager.GetInput() != nullptr, "null input");

	_manager.GetRender()->UpdateEvents();
	_manager.GetInput()->UpdateKeys();
	this->UpdateTimers();
}




void Emulator::CleanFlags()
{
	_instrf = false;
	_drawf = false;
	_exitf = false;
	_manager.CleanFlags();
}



void Emulator::Reset()
{
	ASSERT_MSG(_manager.GetSound() != nullptr, "null sound");

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

	const iRender* oldRend = _manager.SwapRender(rend.release()); 
	if (oldRend)
		delete oldRend;

	if (!InitRender()) 
	{
		_exitf = true;
		return false;
	}

	return true;
}




bool Emulator::SetInput(UniqueInput input) 
{
	utility::LOG("Setting new iInput...");
	const iInput* oldInput = _manager.SwapInput(input.release()); 
	if (oldInput)
		delete oldInput;

	if (!InitInput())
	{
		_exitf = true;
		return false;
	}

	return true;	
}




bool Emulator::SetSound(UniqueSound sound) 
{
	utility::LOG("Setting new iSound...");

	const iSound* oldSound = _manager.SwapSound(sound.release()); 
	if (oldSound)
		delete oldSound;

	if (!InitSound())
	{
		_exitf = true;
		return false;
	}

	return true;
}




UniqueRender Emulator::SwapRender(UniqueRender rend) 
{ 
	if (rend) 
	{
		utility::LOG("Swapping iRender...");
		auto* const oldRend = _manager.SwapRender(rend.release());
		if (!InitRender())
			_exitf = true;

		return UniqueRender(oldRend);
	}

	_exitf = true;
	utility::LOG("Swapping iRender to nullptr...");
	return UniqueRender(_manager.SwapRender(nullptr));
}




UniqueInput Emulator::SwapInput(UniqueInput input) 
{ 
	if (input)
	{
		utility::LOG("Swapping iInput...");
		auto* const oldInput =  _manager.SwapInput(input.release());
		if (!InitInput())
			_exitf = true;

		return UniqueInput(oldInput);
	}

	_exitf = true;
	utility::LOG("Swapping iInput to nullptr...");
	return UniqueInput(_manager.SwapInput(nullptr));
}




UniqueSound Emulator::SwapSound(UniqueSound sound) 
{ 
	if (sound)
	{
		utility::LOG("Swapping iSound...");
		auto* const oldSound(_manager.SwapSound(sound.release()));
		if (!InitInput())
			_exitf = true;

		return UniqueSound(oldSound);
	}

	_exitf = true;
	utility::LOG("Swapping iSound to nullptr...");
	return UniqueSound(_manager.SwapSound(nullptr));
}















bool Emulator::InitRender()
{
	iRender* const rend = _manager.GetRender();

	if (!rend)  {
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
	rend->SetWinCloseCallback(&_exitf, [](const void* exitf) { *(bool*)exitf = true; });
	return true;
}




bool Emulator::InitInput()
{
	iInput* const input = _manager.GetInput();

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


	input->SetEscapeKeyCallback(&_exitf, [](const void* exitf) { *(bool*)exitf = true; });
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
	iSound* const sound = _manager.GetSound();

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













}
