#ifndef CHIP8_INSTRUCTIONS_H
#define CHIP8_INSTRUCTIONS_H


class Chip8;
namespace Chip8Instructions
{
	
	using InstrPtr = void(*)(Chip8*const);
	using InstrTable = void(**)(Chip8*const);
	static InstrTable s_instrTbl;
	
	static bool Initialize();
	static void Dispose() noexcept;
	static void UnknownOpcode(Chip8 *const);


	static void op_0xxx(Chip8 *const); // 3 instructions for 0xxx
	static void op_1NNN(Chip8 *const); // jumps to address NNN
	static void op_2NNN(Chip8 *const); // calls subroutine at NNN
	static void op_3XNN(Chip8 *const); // Skips the next instruction if VX equals NN
	static void op_4XNN(Chip8 *const); // Skips the next instruction if VX doesn't equal NN
	static void op_5XY0(Chip8 *const); // Skips the next instruction if VX equals VY
	static void op_6XNN(Chip8 *const); // Sets VX to NN
	static void op_7XNN(Chip8 *const); // adds NN to VX
	static void op_8XYx(Chip8 *const); // 9 instructions , 8XY0 - 8XY7, + 8XYE
	static void op_9XY0(Chip8 *const); // Skips the next instruction if VX doesn't equal VY
	static void op_ANNN(Chip8 *const); // Sets I to the address NNN
	static void op_BNNN(Chip8 *const); // Jumps to the address NNN plus V0
	static void op_CXNN(Chip8 *const); // Sets VX to the result of a bitwise AND operation on a random number and NN
	static void op_DXYN(Chip8 *const); // DRAW Instruction .....
	static void op_EXxx(Chip8 *const); // 2 instruction EX9E, EXA1
	static void op_FXxx(Chip8 *const); // 9 instructions.
};











#endif
