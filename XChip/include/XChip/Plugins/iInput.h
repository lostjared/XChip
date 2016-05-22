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

#ifndef _XCHIP_IINPUT_H_
#define _XCHIP_IINPUT_H_
#include <XChip/Utility/StdintDef.h>
#include "iPlugin.h"


 
namespace xchip {

enum class Key : uint8_t
{
	//Chip8 keypad, which will be emulated in the computer's keypad
	KEY_0,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,
	KEY_A, KEY_B, KEY_C,
	KEY_D, KEY_E, KEY_F,


	// system keys
	RESET,
	ESCAPE,

	// does not count as a key, but is returned if none of the others are pressed
	NO_KEY_PRESSED
};



class iInput : public iPlugin
{
public:
	using WaitKeyCallback = bool(*)(const void*);
	using ResetKeyCallback = void(*)(const void*);
	using EscapeKeyCallback = void(*)(const void*);

	virtual bool Initialize() noexcept = 0;
	virtual Key GetPressedKey() const noexcept = 0;
	virtual bool IsKeyPressed(const Key key) const noexcept = 0;
	virtual bool UpdateKeys() noexcept = 0;
	virtual Key WaitKeyPress() noexcept = 0;
	
	
	virtual void SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept = 0;
	virtual void SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept = 0;
	virtual void SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept = 0;
};















}

#endif
