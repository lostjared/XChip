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


#ifndef __XCHIP_INSTRUCTIONS_H__
#define __XCHIP_INSTRUCTIONS_H__
#include "CpuManager.h"
 
namespace xchip { namespace instructions {



using InstrTable = void(*)(CpuManager&);
extern InstrTable instrTable[16];

extern void execute_instruction(CpuManager&);


// Primary table
extern void op_0xxx(CpuManager&); // 3 instructions switch
extern void op_1NNN(CpuManager&); // jumps to address NNN
extern void op_2NNN(CpuManager&); // calls subroutine at NNN
extern void op_3XNN(CpuManager&); // Skips the next instruction if VX equals NN
extern void op_4XNN(CpuManager&); // Skips the next instruction if VX doesn't equal NN
extern void op_5XY0(CpuManager&); // Skips the next instruction if VX equals VY
extern void op_6XNN(CpuManager&); // Sets VX to NN
extern void op_7XNN(CpuManager&); // adds NN to VX
extern void op_9XY0(CpuManager&); // Skips the next instruction if VX doesn't equal VY
extern void op_ANNN(CpuManager&); // Sets I to the address NNN
extern void op_BNNN(CpuManager&); // Jumps to the address NNN plus V0
extern void op_CXNN(CpuManager&); // Sets VX to the result of a bitwise AND operation on a random number and NN
extern void op_DXYN(CpuManager&); // DRAW Instruction .....
extern void op_DXYN_ex(CpuManager&); // DRAW Instruction extended mode
extern void op_EXxx(CpuManager&); // 2 instruction EX9E, EXA1
// Primary table end




// 8XYx subtable start
extern void op_8XYx(CpuManager&); // 9 instructions , 8XY0 - 8XY7, + 8XYE
extern void op_8XY0(CpuManager&); // Sets VX to the value of VY.
extern void op_8XY1(CpuManager&); // Sets VX to VX or VY.
extern void op_8XY2(CpuManager&); // Sets VX to VX and VY.
extern void op_8XY3(CpuManager&); // Sets VX to VX xor VY.
extern void op_8XY4(CpuManager&); // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
extern void op_8XY5(CpuManager&); // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
extern void op_8XY6(CpuManager&); // Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
extern void op_8XY7(CpuManager&); // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
extern void op_8XYE(CpuManager&); // Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
// 8XYx subtable end



// FXxxx subtable start
extern void op_FXxx(CpuManager&); // 9 instructions, FX07 - FX33 
extern void op_FX30(CpuManager&); // FX30* SuperChip: Point I to the location of the sprite for the character in VX
extern void op_FX07(CpuManager&); // FX07   Sets VX to the value of the delay timer.
extern void op_FX0A(CpuManager&); // FX0A   A key press is awaited, and then stored in VX.
extern void op_FXx5(CpuManager&); // 3 instructions switch
extern void op_FX18(CpuManager&); // FX18   Sets the sound timer to VX.
extern void op_FX1E(CpuManager&); // FX1E   Adds VX to I.
extern void op_FX29(CpuManager&); // FX29  Sets I to the location of the sprite for the character in VX. 
extern void op_FX33(CpuManager&); // BCD
// FXxxx subtable end







}}

#endif
