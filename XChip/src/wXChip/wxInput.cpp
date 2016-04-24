
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <XChip/Utility/Log.h>
#include <wXChip/wxInput.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>


namespace xchip {

	using namespace utility;


	wxInput::wxInput() noexcept
	/*
	_keyPairs
	{
		{ Key::KEY_0, SDL_SCANCODE_KP_0 },{ Key::KEY_1, SDL_SCANCODE_KP_7 },{ Key::KEY_2, SDL_SCANCODE_KP_8 },
		{ Key::KEY_3, SDL_SCANCODE_KP_9 },{ Key::KEY_4, SDL_SCANCODE_KP_4 },{ Key::KEY_5, SDL_SCANCODE_KP_5 },
		{ Key::KEY_6, SDL_SCANCODE_KP_6 },{ Key::KEY_7, SDL_SCANCODE_KP_1 },{ Key::KEY_8, SDL_SCANCODE_KP_2 },
		{ Key::KEY_9, SDL_SCANCODE_KP_3 },{ Key::KEY_A, SDL_SCANCODE_KP_DIVIDE },{ Key::KEY_B, SDL_SCANCODE_KP_MULTIPLY },
		{ Key::KEY_C, SDL_SCANCODE_KP_MINUS },{ Key::KEY_D, SDL_SCANCODE_KP_PLUS },{ Key::KEY_E, SDL_SCANCODE_KP_PERIOD },
		{ Key::KEY_F, SDL_SCANCODE_KP_ENTER },{ Key::RESET, SDL_SCANCODE_RETURN }, { Key::ESCAPE, SDL_SCANCODE_ESCAPE   }
	}*/

	{
		LOG("Creating wxInput object...");
	}


	wxInput::~wxInput()
	{
		//if (_keyboardState)
		//	this->Dispose();
		
		LOG("Destroying wxInput object...");
	}




	bool wxInput::Initialize() noexcept
	{
		if (_initialized)
			this->Dispose();
		
			
			
			_initialized = true;
		return true;
	}



	void wxInput::Dispose() noexcept
	{
		//_keyboardState = nullptr;
		_resetClbk = nullptr;
		_escapeClbk = nullptr;
		_waitClbk = nullptr;
		_initialized = false;
	}


	bool wxInput::UpdateKeys() noexcept
	{
		
		return false;
	}


	bool wxInput::IsInitialized() const noexcept
	{
		return true;
	}

	bool wxInput::IsKeyPressed(const Key key) const noexcept
	{
		
		
		return true;
	}


	Key wxInput::GetPressedKey() const noexcept
	{

		return Key::NO_KEY_PRESSED;
	}



	Key wxInput::WaitKeyPress() noexcept
	{
		
		return Key::NO_KEY_PRESSED;
	}



	void wxInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
	{
		_waitClbkArg = arg;
		_waitClbk = callback;
	}


	void wxInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
	{
		_resetClbkArg = arg;
		_resetClbk = callback;
	}


	void wxInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
	{
		_escapeClbkArg = arg;
		_escapeClbk = callback;
	}

}