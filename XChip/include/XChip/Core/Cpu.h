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



#ifndef __XCHIP_CPU_H__
#define __XCHIP_CPU_H__
#include <XChip/Utils/Ints.h>
 

namespace xchip {
class iRender;
class iInput;
class iSound;



struct Cpu
{
	uint8_t* memory;
	uint8_t* registers;
	size_t*  stack;
	uint32_t* gfx;

	iRender* render;
	iInput* input;
	iSound* sound;

	size_t sp;
	size_t pc;
	size_t I;

	uint32_t flags;
	uint16_t opcode;
	uint8_t delayTimer;
	uint8_t soundTimer;
	
	enum Flags : uint32_t 
	{ 
		EXIT  = 0x01,
		PAUSE = 0x02,
		INSTR = 0x04,
		DRAW  = 0x08,
		EXTENDED_MODE = 0x10,
		BAD_RENDER = 0x20,
		BAD_INPUT = 0x40,
		BAD_SOUND = 0x80
	};
};







}









#endif
