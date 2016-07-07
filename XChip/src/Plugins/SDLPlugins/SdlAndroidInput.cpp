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
#include <XChip/Plugins/SDLPlugins/SdlAndroidInput.h>


#define _SDLANDROIDINPUT_INITIALIZED_ASSERT_() ASSERT_MSG(m_initialized == true, "SdlAndroidInput is not initialized")

namespace xchip {

using namespace utix;


constexpr const char* const SdlAndroidInput::PLUGIN_NAME;
constexpr const char* const SdlAndroidInput::PLUGIN_VER;




SdlAndroidInput::SdlAndroidInput() noexcept
{
	Log("Creating SdlAndroidInput object...");
}


SdlAndroidInput::~SdlAndroidInput()
{
	if (m_initialized)
		this->Dispose();

	Log("Destroying SdlAndroidInput object...");
}




bool SdlAndroidInput::Initialize() noexcept
{
	// TODO: implement
	if (m_initialized)
		this->Dispose();

	m_initialized = true;
	return true;
}



void SdlAndroidInput::Dispose() noexcept
{
	m_resetClbk = nullptr;
	m_escapeClbk = nullptr;
	m_waitClbk = nullptr;
	m_initialized = false;
}


bool SdlAndroidInput::IsInitialized() const noexcept 
{ 
	return m_initialized; 
}


const char* SdlAndroidInput::GetPluginName() const noexcept
{
	return PLUGIN_NAME;
}

const char* SdlAndroidInput::GetPluginVersion() const noexcept
{
	return PLUGIN_VER;
}

PluginDeleter SdlAndroidInput::GetPluginDeleter() const noexcept
{
	return nullptr;
}


bool SdlAndroidInput::IsKeyPressed(const Key key) const noexcept
{
	_SDLANDROIDINPUT_INITIALIZED_ASSERT_();
	return (key == Key::KEY_6 && m_direction == RIGHT) || (key == Key::KEY_4 && m_direction == LEFT);
}



bool SdlAndroidInput::UpdateKeys() noexcept
{
	_SDLANDROIDINPUT_INITIALIZED_ASSERT_();
	
	constexpr const auto finger_event = SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP;
		
	while(SDL_PollEvent(&m_sdlevent)) 
	{
		if((m_sdlevent.type & finger_event) != 0)
			m_direction = m_sdlevent.button.x > m_middleScreen ? RIGHT : LEFT;
		else if( m_sdlevent.type == SDL_MOUSEMOTION)
			m_direction = m_sdlevent.motion.x > m_middleScreen ? RIGHT : LEFT;
	}

	return true;
}







Key SdlAndroidInput::WaitKeyPress() noexcept
{
	_SDLANDROIDINPUT_INITIALIZED_ASSERT_();
	// TODO: implement
	return Key::NO_KEY_PRESSED;
}





void SdlAndroidInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
{
	m_waitClbkArg = arg;
	m_waitClbk = callback;
}


void SdlAndroidInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
{
	m_resetClbkArg = arg;
	m_resetClbk = callback;
}


void SdlAndroidInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
{
	m_escapeClbkArg = arg;
	m_escapeClbk = callback;
}











}



