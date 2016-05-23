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

#include <SDL2/SDL_events.h>
#include <XChip/Plugins/SDLPlugins/SdlInput.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>


#define _SDLINPUT_INITIALIZED_ASSERT_() ASSERT_MSG(_initialized == true, "SdlInput is not initialized")

namespace xchip {
using namespace utility;


extern "C" void XCHIP_FreePlugin(const iPlugin*);








SdlInput::SdlInput() noexcept
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
	using namespace utility::literals;

	if (_initialized)
		this->Dispose();

	// TODO: Initialize events

	_keyboardState = SDL_GetKeyboardState(NULL);

	if (!_keyboardState) 
	{
		LOGerr("Cannot get Keyboard State");
		return false;
	}

	try
	{
		_keyPairs = std::vector<KeyPair>
		{
			{ Key::KEY_0, SDL_SCANCODE_KP_0 },{ Key::KEY_1, SDL_SCANCODE_KP_7 },{ Key::KEY_2, SDL_SCANCODE_KP_8 },
			{ Key::KEY_3, SDL_SCANCODE_KP_9 },{ Key::KEY_4, SDL_SCANCODE_KP_4 },{ Key::KEY_5, SDL_SCANCODE_KP_5 },
			{ Key::KEY_6, SDL_SCANCODE_KP_6 },{ Key::KEY_7, SDL_SCANCODE_KP_1 },{ Key::KEY_8, SDL_SCANCODE_KP_2 },
			{ Key::KEY_9, SDL_SCANCODE_KP_3 },{ Key::KEY_A, SDL_SCANCODE_KP_DIVIDE },{ Key::KEY_B, SDL_SCANCODE_KP_MULTIPLY },
			{ Key::KEY_C, SDL_SCANCODE_KP_MINUS },{ Key::KEY_D, SDL_SCANCODE_KP_PLUS },{ Key::KEY_E, SDL_SCANCODE_KP_PERIOD },
			{ Key::KEY_F, SDL_SCANCODE_KP_ENTER },{ Key::RESET, SDL_SCANCODE_RETURN },{ Key::ESCAPE, SDL_SCANCODE_ESCAPE }
		};
	}

	catch (const std::exception& err)
	{
		utility::LOGerr("Could not initialize SdlInput::_keyPairs : "_s + err.what());
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


bool SdlInput::IsInitialized() const noexcept 
{ 
	return _initialized; 
}


const char* SdlInput::GetPluginName() const noexcept
{
	return PLUGIN_NAME;
}

const char* SdlInput::GetPluginVersion() const noexcept
{
	return PLUGIN_VER;
}

PluginDeleter SdlInput::GetPluginDeleter() const noexcept
{
	return XCHIP_FreePlugin;
}


bool SdlInput::IsKeyPressed(const Key key) const noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();
	return _keyboardState[_keyPairs[static_cast<size_t>(key)].second] == SDL_TRUE;
}



Key SdlInput::GetPressedKey() const noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();
	for (const auto& keyPair : _keyPairs)
	{
		if (_keyboardState[keyPair.second])
			return keyPair.first;
	}

	return Key::NO_KEY_PRESSED;
}




bool SdlInput::UpdateKeys() noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();

	SDL_PumpEvents();
	_keyboardState = SDL_GetKeyboardState(NULL);

	if (_keyboardState[SDL_SCANCODE_RETURN])
	{
		if (_resetClbk) 
			_resetClbk(_resetClbkArg);

		return false;
	}


	else if (_keyboardState[SDL_SCANCODE_ESCAPE])
	{
		if (_escapeClbk) 
			_escapeClbk(_escapeClbkArg);

		return false;
	}

	return true;
}







Key SdlInput::WaitKeyPress() noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();
	if (_waitClbk != nullptr)
	{
		const auto begin = _keyPairs.crbegin();
		const auto end = _keyPairs.crend();
		
		while (true)
		{
			if(!_waitClbk(_waitClbkArg))
				break;
			if (UpdateKeys())
			{
				for (auto itr = begin; itr != end; ++itr)
				{
					if (_keyboardState[itr->second])
						return itr->first;
				}
			}
		}

	}

	return Key::NO_KEY_PRESSED;
}



void SdlInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
{
	_waitClbkArg = arg;
	_waitClbk = callback;
}


void SdlInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
{
	_resetClbkArg = arg;
	_resetClbk = callback;
}


void SdlInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
{
	_escapeClbkArg = arg;
	_escapeClbk = callback;
}






extern "C" XCHIP_EXPORT iPlugin* XCHIP_LoadPlugin()
{
	return new(std::nothrow) SdlInput();
}



extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin)
{
	const auto* sdlinput = dynamic_cast<const SdlInput*>( plugin );
	if(! sdlinput )
	{
		utility::LOGerr("XCHIP_FreePlugin: dynamic_cast iMediaPlugin to sdlinput failed!");
		std::exit(EXIT_FAILURE);
	}
	
	delete sdlinput;
}




}



