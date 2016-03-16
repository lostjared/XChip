#include <XChip/Instructions.h>
#include <XChip/Cpu.h>
#include <XChip/Utility/Log.h>

// avoid too much identation ...
#define _XCHIP_INSTRUCTIONS_NAMESPACE_ namespace instructions {
XCHIP_NAMESPACE
_XCHIP_INSTRUCTIONS_NAMESPACE_






InstrTable instrTable[16] = 
{
	op_0xxx, op_1NNN, op_2NNN, op_3XNN, 
	op_4XNN, op_5XY0, op_6XNN, op_7XNN, 
	op_8XYx, op_9XY0, op_ANNN, op_BNNN,
	op_CXNN, op_DXYN, op_EXxx, op_FXxx
};


void unknown_opcode(Cpu* const xcpu)
{
	LOGerr("Unknown opcode: " << xcpu->opcode);
}


#define VF  (xcpu->registers [0xF])
#define VX  (xcpu->registers [ ( (xcpu->opcode & 0x0f00 ) >> 8)  ])
#define VY  (xcpu->registers [ ( (xcpu->opcode & 0x00f0 ) >> 4)  ])
#define NNN (xcpu->opcode & 0x0fff)
#define NN  (xcpu->opcode & 0x00ff)
#define N   (xcpu->opcode & 0x000f)



void op_0xxx(Cpu* const xcpu)
{
	switch (xcpu->opcode)
	{
		default: // 0NNN " calls RCA 1802 program at address NNN. not necessary for most ROMs. "
			unknown_opcode(xcpu);
			break;

		case 0x00E0: // clear screen
			//std::fill_n(xcpu->gfx, xcpu->gfxsz, 0);
			break;

		case 0x00EE: // return from a subroutine ( unwind stack )
			xcpu->pc = xcpu->stack[--xcpu->sp];
			break;
	}
}



// 1NNN:  jumps to address NNN
void op_1NNN(Cpu *const xcpu)
{
	xcpu->pc = NNN;
}




// 2NNN: Calls subroutine at address NNN
void op_2NNN(Cpu *const xcpu)
{
	xcpu->stack[xcpu->sp++] = xcpu->pc;
	xcpu->pc = NNN;
}



// 3XNN: Skips the next instruction if VX equals NN
void op_3XNN(Cpu *const xcpu)
{
	if (VX == NN)
		xcpu->pc += 2;
}



// 4XNN: Skips the next instruction if VX doesn't equal NN
void op_4XNN(Cpu *const xcpu)
{
	if (VX != NN)
		xcpu->pc += 2;
}



// 5XY0: Skips the next instruction if VX equals VY
void op_5XY0(Cpu *const xcpu)
{
	if (VX == VY)
		xcpu->pc += 2;
}


// 6XNN: store number NN in register VX
void op_6XNN(Cpu *const xcpu)
{
	VX = NN;
}

// 7XNN: add the value NN to register VX
void op_7XNN(Cpu *const xcpu)
{
	VX = ((VX + NN) & 0xFF);
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
	op_8XYE,
	unknown_opcode
};

void op_8XYx(Cpu* const xcpu)
{
	// call it
	op_8XYx_Table[xcpu->opcode & 0x000f](xcpu);

}




// 8XY0: store the value of register VY in register VX
void op_8XY0(Cpu *const xcpu)
{
	VX = VY;
}






// 8XY1: set VX to VX | VY
void op_8XY1(Cpu *const xcpu)
{
	VX = (VY | VX);
}







// 8XY2: sets VX to VX and VY
void op_8XY2(Cpu *const xcpu)
{
	VX = (VY & VX);
}






// 8XY3: sets VX to VX xor VY
void op_8XY3(Cpu *const xcpu)
{
	VX = ((VY ^ VX) & 0xFF);
}






// 8XY4: Adds VY to VX . VF is set to 1 when theres a carry, and to 0 when there isn't
void op_8XY4(Cpu *const xcpu)
{
		
	uint16_t result = VX + VY; // compute sum
	xcpu->registers[0xF] = ((result & 0xff00) != 0) ? 1 : 0; // check carry
	VX = (result & 0xff);
}

	
	



	
// 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY5(Cpu *const xcpu)
{
		
	xcpu->registers[0xF] = (VX > VY); // checking if theres is a borrow
	VX -= VY;
}

	




	
	
// 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
void op_8XY6(Cpu *const xcpu)
{
	xcpu->registers[0xF] = (VX & 0x1); // check the least significant bit
	VX >>= 1;
}







// 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY7(Cpu *const xcpu)
{
	xcpu->registers[0xF] = (VY > VX); // check borrow ( VY > VX )
	VX = (VY - VX);
}

	




	
// 8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
void op_8XYE(Cpu *const xcpu)
{
	xcpu->registers[0xF] = ((VX & 0x80) >> 7);  // check the most significant bit
	VX = ((VX << 1) & 0xFF);
}


/******** OP_8XYEx END *********/






// 9XY0: skips the next instruction if VX doesn't equal VY
void op_9XY0(Cpu *const xcpu)
{
	if (VX != VY)
		xcpu->pc += 2;
}



// ANNN: sets I to the address NNN
void op_ANNN(Cpu *const xcpu)
{
	xcpu->I = NNN;
}



// BNNN: jumps to the address NNN plus V0
void op_BNNN(Cpu *const xcpu)
{
	xcpu->pc = ((NNN + xcpu->registers[0]) & 0xFFFF);
}



// CXNN: Sets VX to a bitwise operation AND ( & ) between NN and a random number
void op_CXNN(Cpu *const xcpu)
{
	VX = ((std::rand() % 0xff) & NN);
}




// DXYN: DRAW INSTRUCTION
void op_DXYN(Cpu *const xcpu)
{
	// from : http://www.multigesture.net/articles/how-to-write-an-emulator-xcpu-8-interpreter/

	/*	Drawing pixels:
	The opcode responsible for drawing to our display is 0xDXYN. The Wikipedia description tells us the following:
	Draws a sprite at coordinate (VX, VY)
	that has a width of 8 pixels and a height of N pixels.
	Each row of 8 pixels is read as bit-coded starting from memory location I;
	I value doesn't change after the execution of this instruction.
	As described above,
	VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
	and to 0 if that doesn't happen.
	As the description of the opcode is telling us,
	the Chip 8 actually draws on the screen by drawing sprites.
	It will give us the location of where the sprite needs to be drawn
	(the opcode tells us which V register we need to check to fetch the X and Y coordinates) and the number of rows (N).
	The width of each sprite is fixed (8 bits / 1 byte).
	The state of each pixel is set by using a bitwise XOR operation.
	This means that it will compare the current pixel state with the current value in the memory.
	If the current value is different from the value in the memory, the bit value will be 1.
	If both values match, the bit value will be 0.
	*/

	xcpu->registers[0xF] = 0;

	uint8_t Vx = VX, Vy = VY;
	int height = N;
	uint8_t *_8bitRow = &xcpu->memory[xcpu->I];

	for (int i = 0; i < height; ++i, ++_8bitRow)
	{
		for (int j = 0; j < 8; ++j)
		{
			int px = ((Vx + j) & 63);
			int py = ((Vy + i) & 31);

			int pixelPos = (64 * py) + px;

			bool pixel = (*_8bitRow & (1 << (7 - j))) != 0;

			xcpu->registers[0xF] |= ((xcpu->gfx[pixelPos] > 0) & pixel);

			xcpu->gfx[pixelPos] ^= (pixel) ? ~0 : 0;
		}
	}
}






// 2 instruction EX9E, EXA1
void op_EXxx(Cpu *const xcpu)
{
	switch (xcpu->opcode & 0x000f)
	{
		default: unknown_opcode(xcpu); break;

		case 0xE: // EX9E  Skips the next instruction if the key stored in VX is pressed.
			//if (xcpu->input->IsKeyPressed((EmulatorKey)VX))
			//	xcpu->pc += 2;
			break;


		case 0x1: // 0xEXA1  Skips the next instruction if the key stored in VX isn't pressed.
		//	if (!xcpu->input->IsKeyPressed((EmulatorKey)VX))
		//		xcpu->pc += 2;
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




void op_FXxx(Cpu *const xcpu) // 9 instructions.
{
	// call it
	op_FXxx_Table[xcpu->opcode & 0x000f](xcpu);
}




// FX07   Sets VX to the value of the delay timer.
void op_FX07(Cpu *const xcpu)
{
	VX = xcpu->delayTimer;
}




// FX0A   A key press is awaited, and then stored in VX.
void op_FX0A(Cpu *const xcpu)
{
	// VX = static_cast<uint8_t>(xcpu->input->WaitKeyPress());
}





void op_FXx5(Cpu *const xcpu)
{
	switch (xcpu->opcode & 0x00ff)
	{
		default: unknown_opcode(xcpu); break;

		case 0x15: // FX15  Sets the delay timer to VX.
			xcpu->delayTimer = VX;
			break;

		case 0x55: //FX55  Stores V0 to VX in memory starting at address I
			std::memcpy(xcpu->memory + xcpu->I, xcpu->registers, ((xcpu->opcode & 0x0f00) >> 8) + 1);
			break;

		case 0x65: //FX65  Fills V0 to VX with values from memory starting at address I.
			std::memcpy(xcpu->registers, xcpu->memory + xcpu->I, ((xcpu->opcode & 0x0f00) >> 8) + 1);
			break;
	}
}





// FX18   Sets the sound timer to VX.
void op_FX18(Cpu *const xcpu)
{
	xcpu->soundTimer = VX;
}




// FX1E   Adds VX to I.
void op_FX1E(Cpu *const xcpu)
{
	xcpu->I = ((xcpu->I + VX) & 0xFFFF);
}



// FX29  Sets I to the location of the sprite for the character in VX. 
void op_FX29(Cpu *const xcpu)
{
	// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
	xcpu->I = VX * 5;
}




// BCD
void op_FX33(Cpu *const xcpu)
{
	uint8_t Vx = VX;
	xcpu->memory[xcpu->I + 2] = Vx % 10;
	xcpu->memory[xcpu->I + 1] = (Vx / 10) % 10;
	xcpu->memory[xcpu->I] = (Vx / 100);
}









/******** OP_FXxx START *********/

NAMESPACE_END
NAMESPACE_END
