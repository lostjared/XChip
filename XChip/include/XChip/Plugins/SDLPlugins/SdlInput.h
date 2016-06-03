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

#ifndef _XCHIP_SDLINPUT_H_
#define _XCHIP_SDLINPUT_H_


#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>
#include <XChip/Plugins/iInput.h>
#include <XChip/Utility/Vector.h>

 

namespace xchip {

	
class SdlInput final : public iInput
{
	struct KeyPair { Key chip8Key; SDL_Scancode sdlKey; };
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
	utility::Vector<KeyPair> _keyPairs;
	const unsigned char* _keyboardState = nullptr;
	WaitKeyCallback _waitClbk = nullptr;
	ResetKeyCallback _resetClbk = nullptr;
	EscapeKeyCallback _escapeClbk = nullptr;
	const void* _waitClbkArg;
	const void* _resetClbkArg;
	const void* _escapeClbkArg;
	bool _initialized = false;

};









}














#endif
