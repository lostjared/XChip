#include <cstdlib>
#include <cstring>

#include <Chip8/Chip8Instructions.h>
#include <Chip8/Chip8.h>
#include <Chip8/Interfaces/iInput.h>
#include <Chip8/Utility/log.h>

Chip8Instructions::InstrTable Chip8Instructions::s_instrTbl = nullptr;
bool Chip8Instructions::Initialize()
{
	LOG("Initializing Instruction Table...");

	s_instrTbl = (InstrTable) std::malloc(sizeof(InstrPtr) * 0x10);
	if (s_instrTbl == nullptr) {
		LOGerr("Cannot allocate memory for the Instruction Table");
		return false;
	}

	s_instrTbl[0] = op_0xxx;
	s_instrTbl[1] = op_1NNN;
	s_instrTbl[2] = op_2NNN;
	s_instrTbl[3] = op_3XNN;
	s_instrTbl[4] = op_4XNN;
	s_instrTbl[5] = op_5XY0;
	s_instrTbl[6] = op_6XNN;
	s_instrTbl[7] = op_7XNN;
	s_instrTbl[8] = op_8XYx;
	s_instrTbl[9] = op_9XY0;
	s_instrTbl[10] = op_ANNN;
	s_instrTbl[11] = op_BNNN;
	s_instrTbl[12] = op_CXNN;
	s_instrTbl[13] = op_DXYN;
	s_instrTbl[14] = op_EXxx;
	s_instrTbl[15] = op_FXxx;

	return true;
}

void Chip8Instructions::Dispose() noexcept
{
	free(s_instrTbl);
	LOG("Destroying Instruction Table...");
}


// 3 instructions in 0xxx
inline void Chip8Instructions::UnknownOpcode(Chip8 *const chip)
{
	LOGerr("Unknown Opcode: " << std::hex << chip->m_opcode);
	chip->m_exitFlag = true;
}



// NNN: address
// NN: 8 bit constant
// N: 4 bit constant
// X and Y: (4-bit value) register identifier
#define VX  (chip->m_V [ ( (chip->m_opcode & 0x0f00 ) >> 8)  ])
#define VY  (chip->m_V [ ( (chip->m_opcode & 0x00f0 ) >> 4)  ])
#define NNN (chip->m_opcode & 0x0fff)
#define NN  (chip->m_opcode & 0x00ff)
#define N   (chip->m_opcode & 0x000f)


void Chip8Instructions::op_0xxx(Chip8* const chip)
{
	switch (chip->m_opcode)
	{
		case 0x0000: // 0NNN " calls RCA 1802 program at address NNN. not necessary for most ROMs. "
			break;

		case 0x00E0: // clear screen
			std::memset(chip->m_gfx, 0, chip->m_gfxBytes);
			break;

		case 0x00EE: // return from a subroutine ( unwind stack )
			chip->m_pc = chip->m_stack[--chip->m_sp];
			if(chip->m_sp > Chip8::STACK_MAX) {
				LOGerr("Stack Underflow");
				chip->m_exitFlag = true;
			}
			break;

		default:
			UnknownOpcode(chip);

	}
}



// 1NNN:  jumps to address NNN
void Chip8Instructions::op_1NNN(Chip8 *const chip)
{
	chip->m_pc = NNN;
}




// 2NNN: Calls subroutine at address NNN
void Chip8Instructions::op_2NNN(Chip8 *const chip)
{
	chip->m_stack[chip->m_sp++] = chip->m_pc;
	chip->m_pc = NNN;

	if (chip->m_sp >= Chip8::STACK_MAX) {
		LOGerr("Stack Overflow!");
		chip->m_exitFlag = true;
	}
	

}



// 3XNN: Skips the next instruction if VX equals NN
void Chip8Instructions::op_3XNN(Chip8 *const chip)
{
	if (VX == NN)
		chip->m_pc += 2;
}



// 4XNN: Skips the next instruction if VX doesn't equal NN
void Chip8Instructions::op_4XNN(Chip8 *const chip) 
{
	if (VX != NN)
		chip->m_pc += 2;
}



// 5XY0: Skips the next instruction if VX equals VY
void Chip8Instructions::op_5XY0(Chip8 *const chip)
{
	if (VX == VY)
		chip->m_pc += 2;
}


// 6XNN: store number NN in register VX
void Chip8Instructions::op_6XNN(Chip8 *const chip)
{
	VX = NN;
}

// 7XNN: add the value NN to register VX
void Chip8Instructions::op_7XNN(Chip8 *const chip)
{
	
	VX = ((VX + NN) & 0xFF);
}



// 9 instructions , 8XY0 - 8XY7, + 8XYE
void Chip8Instructions::op_8XYx(Chip8 *const chip)
{
	switch (chip->m_opcode & 0x000f)
	{
		
		default: UnknownOpcode(chip); break;


		case 0x0: // 8XY0: store the value of register VY in register VX
			VX = VY;
			break;


		case 0x1: // 8XY1: set VX to VX | VY
			VX = (VY | VX);
			break;

		case 0x2: // 8XY2: sets VX to VX and VY
			VX = (VY & VX);
			break;


		case 0x3: // 8XY3: sets VX to VX xor VY
			VX = ((VY ^ VX) & 0xFF);
			break;


		case 0x4: // 8XY4: Adds VY to VX . VF is set to 1 when theres a carry, and to 0 when there isn't
		{
			int result = VX + VY; // compute sum
			chip->m_V[0xF] = ((result & 0xff00) != 0) ? 1 : 0; // check carry
			VX = (result & 0xff);
			break;
		}



		case 0x5: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			chip->m_V[0xF] = (VX > VY); // checking if theres is a borrow
			VX -= VY;
			break;





		case 0x6: // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
			chip->m_V[0xF] = (VX & 0x1); // check the least significant bit
			VX >>= 1;
			break;





		case 0x7: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			chip->m_V[0xF] = (VY > VX); // check borrow ( VY > VX )
			VX = (VY - VX);
			break;




		case 0xE: // 8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
			chip->m_V[0xF] = ((VX & 0x80) >> 7);  // check the most significant bit
			VX = ((VX << 1) & 0xFF);
			break;
	}


}



// 9XY0: skips the next instruction if VX doesn't equal VY
void Chip8Instructions::op_9XY0(Chip8 *const chip)
{
	if (VX != VY)
		chip->m_pc += 2;
}



// ANNN: sets I to the address NNN
void Chip8Instructions::op_ANNN(Chip8 *const chip)
{	
	chip->m_I = NNN;
}



// BNNN: jumps to the address NNN plus V0
void Chip8Instructions::op_BNNN(Chip8 *const chip)
{
	chip->m_pc = ((NNN + chip->m_V[0]) & 0xFFFF);
}



// CXNN: Sets VX to a bitwise operation AND ( & ) between NN and a random number
void Chip8Instructions::op_CXNN(Chip8 *const chip)
{
	VX = ((std::rand() % 0xff) & NN);
}




// DXYN: DRAW INSTRUCTION
void Chip8Instructions::op_DXYN(Chip8 *const chip)
{
	// from : http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

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

	chip->m_V[0xF] = 0;

	uint8_t Vx = VX, Vy = VY;
	int height = N;
	uint8_t *_8bitRow = &chip->m_memory[chip->m_I];

	for (int i = 0; i < height; ++i, ++_8bitRow)
	{
		for (int j = 0; j < 8; ++j)
		{
			int px = ((Vx + j) & 63);
			int py = ((Vy + i) & 31);

			int pixelPos = (64 * py) + px;

			bool pixel = (*_8bitRow & (1 << (7 - j))) != 0;

			chip->m_V[0xF] |= ((chip->m_gfx[pixelPos] > 0) & pixel);

			chip->m_gfx[pixelPos] ^= (pixel) ? ~0 : 0;
		}
	}
}






// 2 instruction EX9E, EXA1
void Chip8Instructions::op_EXxx(Chip8 *const chip)
{
	switch (chip->m_opcode & 0x000f)
	{
		default: UnknownOpcode(chip); break;

		case 0xE: // EX9E  Skips the next instruction if the key stored in VX is pressed.
			if (chip->m_input->IsKeyPressed((EmulatorKey)VX))
				chip->m_pc += 2;
			break;


		case 0x1: // 0xEXA1  Skips the next instruction if the key stored in VX isn't pressed.
			if (!chip->m_input->IsKeyPressed((EmulatorKey)VX))
				chip->m_pc += 2;
			break;
	}
}



// 9 instructions.
void Chip8Instructions::op_FXxx(Chip8 *const chip)
{
	switch (chip->m_opcode & 0x000f)
	{
		default: UnknownOpcode(chip); break;

		case 0x7: // FX07   Sets VX to the value of the delay timer.
			VX = chip->m_delayTimer;
			break;


		case 0xA: // FX0A   A key press is awaited, and then stored in VX.
			VX = static_cast<uint8_t>(chip->m_input->WaitKeyPress());
			break;

		case 0x8: // FX18   Sets the sound timer to VX.
			chip->m_soundTimer = VX;
			break;



		case 0xE: // FX1E   Adds VX to I.
			chip->m_I = ((chip->m_I + VX) & 0xFFFF);
			break;

		case 0x9: // FX29  Sets I to the location of the sprite for the character in VX. 
			// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			chip->m_I = VX * 5;
			break;


		case 0x3: // FX33   Stores the Binary - coded decimal representation of VX, 
		{
			// with the most significant of three digits at the address in I, 
			// the middle digit at I plus 1, and the least significant digit at I plus 2. 
			// (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
			// the tens digit at location I + 1, and the ones digit at location I + 2.)
			uint8_t Vx = VX;
			chip->m_memory[chip->m_I + 2] = Vx % 10;
			chip->m_memory[chip->m_I + 1] = (Vx / 10) % 10;
			chip->m_memory[chip->m_I] = (Vx / 100);

			break;
		}

		case 0x0: // FX30
			chip->m_I = VX;
			break;

		
		case 0x5: // BEGIN OF FX*5
		{	
			switch (chip->m_opcode & 0x00ff)
			{
				default: UnknownOpcode(chip); break;

				case 0x15: // FX15  Sets the delay timer to VX.
					chip->m_delayTimer = VX;
					break;

				case 0x55: //FX55  Stores V0 to VX in memory starting at address I
					std::memcpy(chip->m_memory + chip->m_I, chip->m_V, ((chip->m_opcode & 0x0f00) >> 8) + 1);
					break;

				case 0x65: //FX65  Fills V0 to VX with values from memory starting at address I.
					std::memcpy(chip->m_V, chip->m_memory + chip->m_I, ((chip->m_opcode & 0x0f00) >> 8) + 1);
					break;

			
			}
			
			break; // END OF FX*5
		}	
	}

}
