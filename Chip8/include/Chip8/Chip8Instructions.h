#ifndef __CHIP8_INSTRUCTIONS_H__
#define __CHIP8_INSTRUCTIONS_H__


class Chip8Cpu;
namespace Chip8Instructions
{
	
	using InstrPtr = void(*)(Chip8Cpu*const);
	using InstrTable = void(**)(Chip8Cpu*const);
	static InstrTable s_instrTbl;
	
	static bool Initialize();
	static void Dispose() noexcept;
	static void UnknownOpcode(Chip8Cpu *const);


	static void op_0xxx(Chip8Cpu *const); // 3 instructions for 0xxx
	static void op_1NNN(Chip8Cpu *const); // jumps to address NNN
	static void op_2NNN(Chip8Cpu *const); // calls subroutine at NNN
	static void op_3XNN(Chip8Cpu *const); // Skips the next instruction if VX equals NN
	static void op_4XNN(Chip8Cpu *const); // Skips the next instruction if VX doesn't equal NN
	static void op_5XY0(Chip8Cpu *const); // Skips the next instruction if VX equals VY
	static void op_6XNN(Chip8Cpu *const); // Sets VX to NN
	static void op_7XNN(Chip8Cpu *const); // adds NN to VX
	static void op_8XYx(Chip8Cpu *const); // 9 instructions , 8XY0 - 8XY7, + 8XYE
	static void op_9XY0(Chip8Cpu *const); // Skips the next instruction if VX doesn't equal VY
	static void op_ANNN(Chip8Cpu *const); // Sets I to the address NNN
	static void op_BNNN(Chip8Cpu *const); // Jumps to the address NNN plus V0
	static void op_CXNN(Chip8Cpu *const); // Sets VX to the result of a bitwise AND operation on a random number and NN
	static void op_DXYN(Chip8Cpu *const); // DRAW Instruction .....
	static void op_EXxx(Chip8Cpu *const); // 2 instruction EX9E, EXA1
	static void op_FXxx(Chip8Cpu *const); // 9 instructions.
};











#endif
