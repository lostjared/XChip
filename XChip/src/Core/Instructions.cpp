#include <algorithm>
#include <XChip/Core/CpuManager.h>
#include <XChip/Core/Instructions.h>
#include <XChip/Media/iInput.h>
#include <XChip/Media/iSound.h>
#include <XChip/Utility/Alloc.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Assert.h>



namespace xchip { namespace instructions {

using utility::arr_size;


#define OPMSN ((_cpu->opcode & 0xf000) >> 12) // opcode most significant nibble
#define X   ((_cpu->opcode & 0x0f00) >> 8)
#define Y   ((_cpu->opcode & 0x00f0) >> 4)
#define N   (_cpu->opcode & 0x000f)
#define NN  (_cpu->opcode & 0x00ff)
#define NNN (_cpu->opcode & 0x0fff)
#define VF  (_cpu->registers [0xF])
#define VX  (_cpu->registers [X])
#define VY  (_cpu->registers [Y])


InstrTable instrTable[16] =
{
	op_0xxx, op_1NNN, op_2NNN, op_3XNN,
	op_4XNN, op_5XY0, op_6XNN, op_7XNN,
	op_8XYx, op_9XY0, op_ANNN, op_BNNN,
	op_CXNN, op_DXYN, op_EXxx, op_FXxx
};




void unknown_opcode(Cpu* const _cpu)
{
	using namespace utility;
	using namespace utility::literals;
	LOGerr("Unknown Opcode: ", Endl::No);
	LOGerr(_cpu->opcode, Fmt::Hex);
	_cpu->errorFlag = true;
}




void execute_instruction(Cpu& _cpu)
{
	ASSERT_MSG(_cpu.memory != nullptr && arr_size(_cpu.memory) >= 0x0FFF, 
                   "Instructions.cpp::execute_instruction: Cpu::memory, null or size is too low!");

	
	ASSERT_MSG(_cpu.registers != nullptr && arr_size(_cpu.registers) >= 0x10,
                   "Instructions.cpp:execute_instruction: Cpu::registers, null or size is too low!");


	ASSERT_MSG(_cpu.stack != nullptr && arr_size(_cpu.stack) >= 0x10,
                   "Instructions.cpp::execute_instruction: Cpu::stack, null or size is too low!");


	ASSERT_MSG(_cpu.gfx != nullptr && arr_size(_cpu.gfx) >= (64 * 32),
                   "Instructions.cpp::execute_instruction: Cpu::Gfx, null or size is too low!");


	ASSERT_MSG((_cpu.pc + 1) < arr_size(_cpu.memory), 
                   "Instructions.cpp::execute_instruction: Cpu::pc greater than Cpu::memory");



	_cpu.opcode =  (_cpu.memory[_cpu.pc] << 8) | _cpu.memory[_cpu.pc + 1];
	_cpu.pc += 2;

	const auto opmsn = static_cast<size_t>((_cpu.opcode & 0xf000)>>12);
	
	ASSERT_MSG(opmsn < arr_size(instrTable), "Instruction Table Overflow!");
	
	instrTable[opmsn](&_cpu);
}




void op_0xxx(Cpu* const _cpu)
{
	switch (_cpu->opcode)
	{
		default: // 0NNN " calls RCA 1802 program at address NNN. not necessary for most ROMs. "
			unknown_opcode(_cpu);
			break;

		case 0x00E0: // clear screen
			std::fill_n(_cpu->gfx, arr_size(_cpu->gfx), 0);
			break;

		case 0x00EE: // return from a subroutine ( unwind stack )
			ASSERT_MSG((_cpu->sp - 1) < arr_size(_cpu->stack),
				"op_00EE: Stack Underflow");

			_cpu->pc = _cpu->stack[--_cpu->sp];
			break;
	}
}



// 1NNN:  jumps to address NNN
void op_1NNN(Cpu *const _cpu)
{
	_cpu->pc = NNN;
}




// 2NNN: Calls subroutine at address NNN
void op_2NNN(Cpu *const _cpu)
{

	ASSERT_MSG(_cpu->sp < arr_size(_cpu->stack),
		"op_2NNN: Stack Overflow");
	
	_cpu->stack[_cpu->sp++] = _cpu->pc;
	_cpu->pc = NNN;
}



// 3XNN: Skips the next instruction if VX equals NN
void op_3XNN(Cpu *const _cpu)
{
	if (VX == NN)
		_cpu->pc += 2;
}



// 4XNN: Skips the next instruction if VX doesn't equal NN
void op_4XNN(Cpu *const _cpu)
{
	if (VX != NN)
		_cpu->pc += 2;
}



// 5XY0: Skips the next instruction if VX equals VY
void op_5XY0(Cpu *const _cpu)
{
	if (VX == VY)
		_cpu->pc += 2;
}


// 6XNN: store number NN in register VX
void op_6XNN(Cpu *const _cpu)
{
	VX = NN;
}

// 7XNN: add the value NN to register VX
void op_7XNN(Cpu *const _cpu)
{
	auto& vx = VX;
	vx = ((vx + NN) & 0xFF);
}




/******** OP_8XYEx START *********/

// 8XYx Subtable
// 0 - 7 ; instructions
// 8 - D ; unknown opcodes
// E     ; last instruction for this table
// F     ; unknown opcode
static InstrTable op_8XYx_Table[16] =
{
	op_8XY0, op_8XY1, op_8XY2, op_8XY3,
	op_8XY4, op_8XY5, op_8XY6, op_8XY7,
	unknown_opcode, unknown_opcode, unknown_opcode,
	unknown_opcode, unknown_opcode, unknown_opcode,
	op_8XYE, unknown_opcode
};

void op_8XYx(Cpu* const _cpu)
{
	ASSERT_MSG(static_cast<size_t>(N) < arr_size(op_8XYx_Table),
		"op_8XYx_Table Overflow!");

	// call it
	op_8XYx_Table[N](_cpu);

}




// 8XY0: store the value of register VY in register VX
void op_8XY0(Cpu *const _cpu)
{
	VX = VY;
}






// 8XY1: set VX to VX | VY
void op_8XY1(Cpu *const _cpu)
{
	auto& vx = VX;
	vx = (VY | vx);
}







// 8XY2: sets VX to VX and VY
void op_8XY2(Cpu *const _cpu)
{
	auto& vx = VX;
	vx = (VY & vx);
}






// 8XY3: sets VX to VX xor VY
void op_8XY3(Cpu *const _cpu)
{
	auto& vx = VX;
	vx = ((VY ^ vx) & 0xFF);
}






// 8XY4: Adds VY to VX . VF is set to 1 when theres a carry, and to 0 when there isn't
void op_8XY4(Cpu *const _cpu)
{
	auto& vx = VX;
	uint16_t result = vx + VY; // compute sum
	VF = (result & 0xff00) != 0 ? 1 : 0; // check carry
	vx = (result & 0xff);
}







// 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY5(Cpu *const _cpu)
{
	auto const vy = VY;
	auto& vx = VX;
	VF = vx > vy; // checking if theres is a borrow
	vx -= vy;
}








// 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
void op_8XY6(Cpu *const _cpu)
{
	auto& vx = VX;
	VF = vx & 0x1; // check the least significant bit
	vx >>= 1;
}







// 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY7(Cpu *const _cpu)
{
	const auto vy = VY;
	auto &vx = VX; 
	VF = vy > vx; // check borrow ( VY > VX )
	vx = vy - vx;
}







// 8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
void op_8XYE(Cpu *const _cpu)
{
	auto& vx = VX;
	VF = (vx & 0x80) >> 7;  // check the most significant bit
	vx = (vx << 1) & 0xFF;
}


/******** OP_8XYEx END *********/










// 9XY0: skips the next instruction if VX doesn't equal VY
void op_9XY0(Cpu *const _cpu)
{
	if (VX != VY)
		_cpu->pc += 2;
}



// ANNN: sets I to the address NNN
void op_ANNN(Cpu *const _cpu)
{
	_cpu->I = NNN;
}



// BNNN: jumps to the address NNN plus V0
void op_BNNN(Cpu *const _cpu)
{
	_cpu->pc = ((NNN + _cpu->registers[0]) & 0xFFFF);
}



// CXNN: Sets VX to a bitwise operation AND ( & ) between NN and a random number
void op_CXNN(Cpu *const _cpu)
{
	VX = ((std::rand() % 0xff) & NN);
}




// DXYN: DRAW INSTRUCTION
void op_DXYN(Cpu *const _cpu)
{

	VF = 0;
	const auto vx = VX;
	const auto vy = VY;
	const int height = N;
	const uint8_t* _8bitRow = _cpu->memory + _cpu->I;

	for (int i = 0; i < height; ++i, ++_8bitRow)
	{
		for (int j = 0; j < 8; ++j)
		{
			const int px = ((vx + j) & 63);
			const int py = ((vy + i) & 31);

			const int pixelPos = (64 * py) + px;

			const bool pixel = (*_8bitRow & (1 << (7 - j))) != 0;

			VF |= ((_cpu->gfx[pixelPos] > 0) & pixel);

			_cpu->gfx[pixelPos] ^= (pixel) ? ~0 : 0;
		}
	}
}






// 2 instruction EX9E, EXA1
void op_EXxx(Cpu *const _cpu)
{
	ASSERT_MSG(_cpu->input != nullptr && _cpu->input->IsInitialized(),
               "Instructions.cpp::op_EXxx: Cpu::Input, null or not initialized!");


	switch (N)
	{
		default: unknown_opcode(_cpu); break;

		case 0xE: // EX9E  Skips the next instruction if the key stored in VX is pressed.
			if (_cpu->input->IsKeyPressed((Key)VX))
				_cpu->pc += 2;
			break;


		case 0x1: // 0xEXA1  Skips the next instruction if the key stored in VX isn't pressed.
			if (!_cpu->input->IsKeyPressed((Key)VX))
				_cpu->pc += 2;
			break;
	}
}








/******** OP_FXxx START *********/

// FXxxx subtable start
static InstrTable op_FXxx_Table[] =
{
	unknown_opcode, unknown_opcode, unknown_opcode,
	op_FX33, unknown_opcode, op_FXx5, unknown_opcode,
	op_FX07, op_FX18, op_FX29, op_FX0A, unknown_opcode,
	unknown_opcode, unknown_opcode, op_FX1E
};




void op_FXxx(Cpu *const _cpu) // 9 instructions.
{
	ASSERT_MSG(static_cast<size_t>(N) < arr_size(op_FXxx_Table),
		"Table op_FXxx_Table overflow...");

	op_FXxx_Table[N](_cpu);
}




// FX07   Sets VX to the value of the delay timer.
void op_FX07(Cpu *const _cpu)
{
	VX = _cpu->delayTimer;
}




// FX0A   A key press is awaited, and then stored in VX.
void op_FX0A(Cpu *const _cpu)
{
	ASSERT_MSG(_cpu->input != nullptr && _cpu->input->IsInitialized(),
               "Instructions.cpp::op_FX0A: Cpu::input, null or not initialized!");

	VX = static_cast<uint8_t>(_cpu->input->WaitKeyPress());
}





void op_FXx5(Cpu *const _cpu)
{
	switch (_cpu->opcode & 0x00ff)
	{
		default: unknown_opcode(_cpu); break;

		case 0x15: // FX15  Sets the delay timer to VX.
			_cpu->delayTimer = VX;
			break;


		case 0x55: //FX55  Stores V0 to VX in memory starting at address I
			ASSERT_MSG(static_cast<size_t>(X+1) < (arr_size(_cpu->memory) - _cpu->I),
				"Instructions.cpp::op_FX55: memory overflow");

			std::copy_n(_cpu->registers, X + 1, _cpu->memory + _cpu->I);
			break;

		case 0x65: //FX65  Fills V0 to VX with values from memory starting at address I.
			ASSERT_MSG(static_cast<size_t>(X+1) < arr_size(_cpu->registers),
				"Instructions.cpp::op_FX65: registers overflow");

			std::copy_n(_cpu->memory + _cpu->I, X + 1, _cpu->registers);
			break;
	}
}





// FX18   Sets the sound timer to VX.
void op_FX18(Cpu *const _cpu)
{
	ASSERT_MSG(_cpu->sound != nullptr && _cpu->sound->IsInitialized(),
               "Instructions::op_FX18: Cpu::sound, null or not initialized");

	_cpu->soundTimer = VX;

	if (_cpu->soundTimer > 0) {
		_cpu->sound->Play(_cpu->soundTimer);
	}
	else {
		if (_cpu->sound->IsPlaying())
			_cpu->sound->Stop();
	}
}




// FX1E   Adds VX to I.
void op_FX1E(Cpu *const _cpu)
{
	_cpu->I = ((_cpu->I + VX) & 0xFFFF);
}



// FX29  Sets I to the location of the sprite for the character in VX. 
void op_FX29(Cpu *const _cpu)
{
	// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
	_cpu->I = VX * 5;
}




// BCD: Stores the Binary-coded decimal representation of VX, with the most significant 
// of three digits at the address in I, the middle digit at I plus 1, 
// and the least significant digit at I plus 2. 
// (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
//  the tens digit at location I+1, and the ones digit at location I+2.)
void op_FX33(Cpu *const _cpu)
{
	ASSERT_MSG(arr_size(_cpu->memory) > (_cpu->I + 2),
		"Instructions.cpp:op_FX33: Cpu::I + 2 overflows Cpu::memory!");

	const uint8_t vx = VX;
	_cpu->memory[_cpu->I + 2] = vx % 10;
	_cpu->memory[_cpu->I + 1] = (vx / 10) % 10;
	_cpu->memory[_cpu->I] = (vx / 100);
}









/******** OP_FXxx END *********/

}}
