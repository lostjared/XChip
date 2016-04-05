#include <SDL2/SDL_events.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Traits.h>
#include <XChip/SDL_MEDIA/SdlInput.h>



namespace xchip {
using namespace utility;

	

SdlInput::SdlInput()
	: SdlMedia(System::Input),
	_keyPairs
	{
		{ Key::KEY_0, SDL_SCANCODE_KP_0 },{ Key::KEY_1, SDL_SCANCODE_KP_7 },{ Key::KEY_2, SDL_SCANCODE_KP_8 },
		{ Key::KEY_3, SDL_SCANCODE_KP_9 },{ Key::KEY_4, SDL_SCANCODE_KP_4 },{ Key::KEY_5, SDL_SCANCODE_KP_5 },
		{ Key::KEY_6, SDL_SCANCODE_KP_6 },{ Key::KEY_7, SDL_SCANCODE_KP_1 },{ Key::KEY_8, SDL_SCANCODE_KP_2 },
		{ Key::KEY_9, SDL_SCANCODE_KP_3 },{ Key::KEY_A, SDL_SCANCODE_KP_DIVIDE },{ Key::KEY_B, SDL_SCANCODE_KP_MULTIPLY },
		{ Key::KEY_C, SDL_SCANCODE_KP_MINUS },{ Key::KEY_D, SDL_SCANCODE_KP_PLUS },{ Key::KEY_E, SDL_SCANCODE_KP_PERIOD },
		{ Key::KEY_F, SDL_SCANCODE_KP_ENTER },{ Key::RESET, SDL_SCANCODE_RETURN }, { Key::ESCAPE, SDL_SCANCODE_ESCAPE   }
	}

{
	LOG("Creating SdlInput object...");
}


SdlInput::~SdlInput()
{
	if (_keyboardState)
		this->Dispose();

	LOG("Destroying SdlInput object...");
}




bool SdlInput::Initialize() noexcept
{
	if (_initialized)
		this->Dispose();

	else if (!this->InitSubSystem())
		return false;

	_keyboardState = SDL_GetKeyboardState(NULL);

	if (!_keyboardState) 
	{
		LOGerr("Cannot get Keyboard State");
		return false;
	}

	_initialized = true;
	return true;
}



void SdlInput::Dispose() noexcept
{
	_keyboardState = nullptr;
	_resetClbk = nullptr;
	_escapeClbk = nullptr;
	_waitClbk = nullptr;
	_initialized = false;
}


bool SdlInput::UpdateKeys()
{
	SdlMedia::UpdateEvents();
	if (GetEvent().type == SDL_KEYDOWN)
	{
		if (GetEvent().key.keysym.sym == SDLK_RETURN)
		{
			if (_resetClbk) 
				_resetClbk(_resetClbkArg);
		}


		else if (GetEvent().key.keysym.sym == SDLK_ESCAPE)
		{
			if (_escapeClbk) 
				_escapeClbk(_escapeClbkArg);
		}

		return true;
	}

	return false;
}



bool SdlInput::IsKeyPressed(const Key key) const
{
	return _keyboardState[_keyPairs[toUType(key)].second] == SDL_TRUE;
}


Key SdlInput::GetPressedKey() const
{
	for (const auto& keyPair : _keyPairs)
	{
		if (_keyboardState[keyPair.second] == SDL_TRUE)
			return keyPair.first;
	}

	return Key::NO_KEY_PRESSED;
}



Key SdlInput::WaitKeyPress()
{
	if (_waitClbk != nullptr)
	{
		const auto begin = _keyPairs.crbegin();
		const auto end = _keyPairs.crend();
		
		while (_waitClbk(_waitClbkArg))
		{
			if (UpdateKeys())
			{
				for (auto itr = begin; itr != end; ++itr)
				{
					if (_keyboardState[itr->second] == SDL_TRUE)
						return itr->first;
				}
			}
		}

	}

	return Key::NO_KEY_PRESSED;
}



void SdlInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) 
{
	_waitClbkArg = arg;
	_waitClbk = callback;
}


void SdlInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) 
{
	_resetClbkArg = arg;
	_resetClbk = callback;
}


void SdlInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback)
{
	_escapeClbkArg = arg;
	_escapeClbk = callback;
}








}



