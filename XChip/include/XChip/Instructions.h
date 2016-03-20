#ifndef __XCHIP_INSTRUCTIONS_H__
#define __XCHIP_INSTRUCTIONS_H__
#include "XChip.h"

_XCHIP_NAMESPACE_

struct Cpu;

namespace instructions 
{
	using InstrTable = void(*)(Cpu* const);
	extern InstrTable instrTable[16];

	extern void op_0xxx(Cpu *const); // 3 instructions switch
	extern void op_1NNN(Cpu *const); // jumps to address NNN
	extern void op_2NNN(Cpu *const); // calls subroutine at NNN
	extern void op_3XNN(Cpu *const); // Skips the next instruction if VX equals NN
	extern void op_4XNN(Cpu *const); // Skips the next instruction if VX doesn't equal NN
	extern void op_5XY0(Cpu *const); // Skips the next instruction if VX equals VY
	extern void op_6XNN(Cpu *const); // Sets VX to NN
	extern void op_7XNN(Cpu *const); // adds NN to VX

	// 8XYx subtable start
	extern void op_8XYx(Cpu *const); // 9 instructions , 8XY0 - 8XY7, + 8XYE
	extern void op_8XY0(Cpu *const); // Sets VX to the value of VY.
	extern void op_8XY1(Cpu *const); // Sets VX to VX or VY.
	extern void op_8XY2(Cpu *const); // Sets VX to VX and VY.
	extern void op_8XY3(Cpu *const); // Sets VX to VX xor VY.
	extern void op_8XY4(Cpu *const); // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
	extern void op_8XY5(Cpu *const); // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
	extern void op_8XY6(Cpu *const); // Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
	extern void op_8XY7(Cpu *const); // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
	extern void op_8XYE(Cpu *const); // Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
	// 8XYx subtable end


	extern void op_9XY0(Cpu *const); // Skips the next instruction if VX doesn't equal VY
	extern void op_ANNN(Cpu *const); // Sets I to the address NNN
	extern void op_BNNN(Cpu *const); // Jumps to the address NNN plus V0
	extern void op_CXNN(Cpu *const); // Sets VX to the result of a bitwise AND operation on a random number and NN
	extern void op_DXYN(Cpu *const); // DRAW Instruction .....
	extern void op_EXxx(Cpu *const); // 2 instruction EX9E, EXA1



	// FXxxx subtable start
	extern void op_FXxx(Cpu *const); // 9 instructions, FX07 - FX33 
	extern void op_FX07(Cpu *const); // FX07   Sets VX to the value of the delay timer.
	extern void op_FX0A(Cpu *const); // FX0A   A key press is awaited, and then stored in VX.
	extern void op_FXx5(Cpu *const); // 3 instructions switch
	extern void op_FX18(Cpu *const); // FX18   Sets the sound timer to VX.
	extern void op_FX1E(Cpu *const); // FX1E   Adds VX to I.
	extern void op_FX29(Cpu *const); // FX29  Sets I to the location of the sprite for the character in VX. 
	extern void op_FX33(Cpu *const); // BCD
	// FXxxx subtable end
	
}



_XCHIP_NAMESPACE_END

#endif
