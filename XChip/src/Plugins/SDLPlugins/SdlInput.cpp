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

#include <cstdlib>
#include <SDL2/SDL_events.h>
#include <Utix/Log.h>
#include <Utix/ScopeExit.h>
#include <Utix/Assert.h>
#include <XChip/Plugins/SDLPlugins/SdlInput.h>

#define _SDLINPUT_INITIALIZED_ASSERT_() ASSERT_MSG(m_initialized == true, "SdlInput is not initialized")

namespace xchip {

using namespace utix;

#ifndef __ANDROID__
extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin*);
#else
#define XCHIP_FreePlugin nullptr
#endif



constexpr const char* const SdlInput::PLUGIN_NAME;
constexpr const char* const SdlInput::PLUGIN_VER;




SdlInput::SdlInput() noexcept
{
	Log("Creating SdlInput object...");
}


SdlInput::~SdlInput()
{
	if (m_initialized)
		this->Dispose();

	Log("Destroying SdlInput object...");
}




bool SdlInput::Initialize() noexcept
{
	if (m_initialized)
		this->Dispose();

	// TODO: Initialize events

	m_keyboardState = SDL_GetKeyboardState(NULL);

	if (!m_keyboardState) 
	{
		LogError("Cannot get Keyboard State: %s", SDL_GetError());
		return false;
	}

	if(m_keyPairs.empty())
	{
		bool ret = m_keyPairs.initialize({
				{ Key::KEY_0, SDL_SCANCODE_KP_0 },{ Key::KEY_1, SDL_SCANCODE_KP_7 },{ Key::KEY_2, SDL_SCANCODE_KP_8 },
				{ Key::KEY_3, SDL_SCANCODE_KP_9 },{ Key::KEY_4, SDL_SCANCODE_KP_4 },{ Key::KEY_5, SDL_SCANCODE_KP_5 },
				{ Key::KEY_6, SDL_SCANCODE_KP_6 },{ Key::KEY_7, SDL_SCANCODE_KP_1 },{ Key::KEY_8, SDL_SCANCODE_KP_2 },
				{ Key::KEY_9, SDL_SCANCODE_KP_3 },{ Key::KEY_A, SDL_SCANCODE_KP_DIVIDE },{ Key::KEY_B, SDL_SCANCODE_KP_MULTIPLY },
				{ Key::KEY_C, SDL_SCANCODE_KP_MINUS },{ Key::KEY_D, SDL_SCANCODE_KP_PLUS },{ Key::KEY_E, SDL_SCANCODE_KP_PERIOD },
				{ Key::KEY_F, SDL_SCANCODE_KP_ENTER } 
			});

		if(!ret)
			return false;
	}

	m_initialized = true;
	return true;
}



void SdlInput::Dispose() noexcept
{
	m_keyboardState = nullptr;
	m_resetClbk = nullptr;
	m_escapeClbk = nullptr;
	m_waitClbk = nullptr;
	m_initialized = false;
}


bool SdlInput::IsInitialized() const noexcept 
{ 
	return m_initialized; 
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

	return m_keyboardState[m_keyPairs[ToSizeT(key)].sdlKey] == SDL_TRUE;
}



bool SdlInput::UpdateKeys() noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();

	SDL_PumpEvents();
	m_keyboardState = SDL_GetKeyboardState(NULL);

	if (m_keyboardState[SDL_SCANCODE_RETURN])
	{
		if (m_resetClbk) 
			m_resetClbk(m_resetClbkArg);

		return true;
	}


	else if (m_keyboardState[SDL_SCANCODE_ESCAPE])
	{
		if (m_escapeClbk) 
			m_escapeClbk(m_escapeClbkArg);

		return true;
	}

	return false;
}







Key SdlInput::WaitKeyPress() noexcept
{
	_SDLINPUT_INITIALIZED_ASSERT_();
	
	if (m_waitClbk != nullptr)
	{
		const auto begin = m_keyPairs.cbegin();
		const auto end = m_keyPairs.cend();
		
		while (m_waitClbk(m_waitClbkArg))
		{
			if (!this->UpdateKeys())
			{
				for (auto itr = begin; itr != end; ++itr)
				{
					if (m_keyboardState[itr->sdlKey])
						return itr->chip8Key;
				}
			}
		}

	}

	return Key::NO_KEY_PRESSED;
}





void SdlInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
{
	m_waitClbkArg = arg;
	m_waitClbk = callback;
}


void SdlInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
{
	m_resetClbkArg = arg;
	m_resetClbk = callback;
}


void SdlInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
{
	m_escapeClbkArg = arg;
	m_escapeClbk = callback;
}













#ifndef __ANDROID__
extern "C" {




XCHIP_EXPORT iPlugin* XCHIP_LoadPlugin()
{
	return new(std::nothrow) SdlInput();
}



XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin)
{
	const auto* sdlinput = dynamic_cast<const SdlInput*>( plugin );
	if(! sdlinput )
	{
		LogError("XCHIP_FreePlugin: dynamic_cast iPlugin to SdlInput failed!");
		std::exit(EXIT_FAILURE);
	}
	
	delete sdlinput;
}







}

#endif // __ANDROID__




}



