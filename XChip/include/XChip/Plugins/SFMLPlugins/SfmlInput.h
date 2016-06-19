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

#ifndef _XCHIP_SFMLINPUT_H_
#define _XCHIP_SFMLINPUT_H_


#include <SFML/Window/Keyboard.hpp>
#include <XChip/Plugins/iInput.h>
#include <Xlib/Vector.h>

namespace xchip {



class SfmlInput final : public iInput
{
	struct KeyPair { Key chip8Key; sf::Keyboard::Key sfKey; };
public:
	using WaitKeyCallback = bool(*)(const void*);
	using ResetKeyCallback = void(*)(const void*);
	using EscapeKeyCallback = void(*)(const void*);

	SfmlInput();
	~SfmlInput();
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
	xlib::Vector<KeyPair> _keyPairs;
	
	const void* _waitArg = nullptr;
	const void* _resetArg = nullptr;
	const void* _escapeArg = nullptr;
	WaitKeyCallback _waitCallback = nullptr;
	ResetKeyCallback _resetCallback = nullptr;
	EscapeKeyCallback _escapeCallback = nullptr;
	bool _initialized = false;
};











}









#endif


