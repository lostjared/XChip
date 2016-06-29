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

#include <Utix/Log.h>
#include <Utix/Assert.h>
#include <Utix/BaseTraits.h>

#include <XChip/Plugins/SFMLPlugins/SfmlInput.h>


#define _SFMLINPUT_INITIALIZED_ASSERT_() ASSERT_MSG(m_initialized, "SfmlInput is not initiaized")

#define _XCHIP_PLUGIN_NAME_ "XCHIP SFML INPUT BY DHUST"
#define _XCHIP_PLUGIN_VER_ "0.1"

namespace xchip {

using namespace utix;

extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin);





SfmlInput::SfmlInput()
{
	Log("Creating " _XCHIP_PLUGIN_NAME_ " Object...");
}

SfmlInput::~SfmlInput()
{
	if(m_initialized)
		this->Dispose();

	Log("Destroying " _XCHIP_PLUGIN_NAME_ " Object...");
}



bool SfmlInput::Initialize() noexcept
{
	if(m_initialized)
		this->Dispose();

	
	if(m_keyPairs.empty())
	{
		bool ret = m_keyPairs.initialize({
			{ Key::KEY_0, sf::Keyboard::Numpad0 }, { Key::KEY_1, sf::Keyboard::Numpad7 }, 
			{ Key::KEY_2, sf::Keyboard::Numpad8 }, { Key::KEY_3, sf::Keyboard::Numpad9 }, 
			{ Key::KEY_4, sf::Keyboard::Numpad4 }, { Key::KEY_5, sf::Keyboard::Numpad5 },
			{ Key::KEY_6, sf::Keyboard::Numpad6 }, { Key::KEY_7, sf::Keyboard::Numpad1 }, 
			{ Key::KEY_8, sf::Keyboard::Numpad2 }, { Key::KEY_9, sf::Keyboard::Numpad3 }, 
			{ Key::KEY_A, sf::Keyboard::Subtract }, { Key::KEY_B, sf::Keyboard::Add },
			{ Key::KEY_C, sf::Keyboard::Divide },  { Key::KEY_D, sf::Keyboard::Multiply },
			{ Key::KEY_E, sf::Keyboard::Comma }, { Key::KEY_F, sf::Keyboard::Period }
		});

		if(!ret)
			return false;
	}

	m_initialized = true;
	return true;

}


void SfmlInput::Dispose() noexcept 
{
	m_waitArg = nullptr;
	m_resetArg = nullptr;
	m_escapeArg = nullptr;
	m_waitCallback = nullptr;
	m_resetCallback = nullptr;
	m_escapeCallback = nullptr;
	m_initialized = false;		
}



bool SfmlInput::IsInitialized() const noexcept
{
	return m_initialized;
}



const char* SfmlInput::GetPluginName() const noexcept
{
	return _XCHIP_PLUGIN_NAME_;
}



const char* SfmlInput::GetPluginVersion() const noexcept
{
	return _XCHIP_PLUGIN_VER_;
}



PluginDeleter SfmlInput::GetPluginDeleter() const noexcept
{
	return XCHIP_FreePlugin;
}



bool SfmlInput::IsKeyPressed(const Key key) const noexcept
{
	_SFMLINPUT_INITIALIZED_ASSERT_();
	if( sf::Keyboard::isKeyPressed(m_keyPairs[ToSizeT(key)].sfKey) )
		return true;

	return false;
}



bool SfmlInput::UpdateKeys() noexcept
{
	_SFMLINPUT_INITIALIZED_ASSERT_();

	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ) {
		m_resetCallback(m_resetArg);
		return false;
	}
	else if( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
		m_escapeCallback(m_escapeArg);
		return false;
	}

	return true;
}


Key SfmlInput::WaitKeyPress() noexcept
{
	_SFMLINPUT_INITIALIZED_ASSERT_();

	if( m_waitCallback )
	{
		while(m_waitCallback(m_waitArg))
		{
			if(UpdateKeys())
			{
				for(const auto& kpair : m_keyPairs)
					if( sf::Keyboard::isKeyPressed(kpair.sfKey) )
						return kpair.chip8Key;
			}
		}
	}

	return Key::NO_KEY_PRESSED;
}



void SfmlInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
{
	m_waitCallback = callback;
	m_waitArg = arg;
}



void SfmlInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
{
	m_resetCallback = callback;
	m_resetArg = arg;
}



void SfmlInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
{
	m_escapeCallback = callback;
	m_escapeArg = arg;
}










extern "C" XCHIP_EXPORT iPlugin* XCHIP_LoadPlugin()
{
	return new(std::nothrow) SfmlInput();
}



extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin)
{
	const auto* sfmlPlugin = dynamic_cast<const SfmlInput*>( plugin );
	if(! sfmlPlugin )
	{
		LogError("XCHIP_FreePlugin: dynamic_cast iPlugin to SfmlInput failed!");
		std::exit(EXIT_FAILURE);
	}
	
	delete sfmlPlugin;
}








}
