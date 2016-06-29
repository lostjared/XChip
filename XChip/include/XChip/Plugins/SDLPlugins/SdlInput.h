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

#ifndef XCHIP_PLUGINS_SDLINPUT_H_
#define XCHIP_PLUGINS_SDLINPUT_H_


#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <XChip/Plugins/iInput.h>
#include <Utix/Vector.h>

 

namespace xchip {

	
class SdlInput final : public iInput
{
	static constexpr const char* const PLUGIN_NAME = "SdlInput";
	static constexpr const char* const PLUGIN_VER = "SdlInput 1.0. Using SDL2";
public:
	SdlInput() noexcept;
	~SdlInput();
	
	bool Initialize() noexcept override;
	void Dispose() noexcept override;
	bool IsInitialized() const noexcept override;
	const char* GetPluginName() const noexcept override;
	const char* GetPluginVersion() const noexcept override;
	PluginDeleter GetPluginDeleter() const noexcept override;
	bool IsKeyPressed(const Key key) const noexcept override;

	bool UpdateKeys() noexcept override;
	Key WaitKeyPress() noexcept override;

	void SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept override;
	void SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept override;
	void SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept override;

private:
	struct KeyPair { Key chip8Key; SDL_Scancode sdlKey; };
	utix::Vector<KeyPair> m_keyPairs;
	const unsigned char* m_keyboardState = nullptr;
	WaitKeyCallback m_waitClbk = nullptr;
	ResetKeyCallback m_resetClbk = nullptr;
	EscapeKeyCallback m_escapeClbk = nullptr;
	const void* m_waitClbkArg;
	const void* m_resetClbkArg;
	const void* m_escapeClbkArg;
	bool m_initialized = false;

};









}














#endif // XCHIP_PLUGINS_SDLINPUT_H_
