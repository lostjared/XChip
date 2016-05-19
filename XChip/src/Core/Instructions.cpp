#include <algorithm>
#include <XChip/Core/Fonts.h>
#include <XChip/Core/CpuManager.h>
#include <XChip/Core/Instructions.h>
#include <XChip/Media/iRender.h>
#include <XChip/Media/iInput.h>
#include <XChip/Media/iSound.h>
#include <XChip/Utility/Alloc.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Assert.h>



namespace xchip { namespace instructions {

using utility::arr_size;


#define OPMSN ((cpuMan.GetOpcode(0xf000) >> 12)) // opcode most significant nibble
#define X   ((cpuMan.GetOpcode(0x0f00) >> 8))
#define Y   ((cpuMan.GetOpcode(0x00f0) >> 4))
#define N   (cpuMan.GetOpcode(0x000f))
#define NN  (cpuMan.GetOpcode(0x00ff))
#define NNN (cpuMan.GetOpcode(0x0fff))
#define VF  (cpuMan.GetRegisters(0xF))
#define VX  (cpuMan.GetRegisters(X))
#define VY  (cpuMan.GetRegisters(Y))


InstrTable instrTable[16] =
{
	op_0xxx, op_1NNN, op_2NNN, op_3XNN,
	op_4XNN, op_5XY0, op_6XNN, op_7XNN,
	op_8XYx, op_9XY0, op_ANNN, op_BNNN,
	op_CXNN, op_DXYN, op_EXxx, op_FXxx
};




void unknown_opcode(CpuManager& cpuMan)
{
	using namespace utility;
	using namespace utility::literals;
	LOGerr("Unknown Opcode: ", Endl::No);
	LOGerr(cpuMan.GetOpcode(), Fmt::Hex);
	cpuMan.SetFlags(Cpu::EXIT);
}




void execute_instruction(CpuManager& cpuMan)
{
	// decode the next opcode 
	cpuMan.SetOpcode(( cpuMan.GetMemory( cpuMan.GetPC() ) << 8 ) | cpuMan.GetMemory(cpuMan.GetPC() + 1) );
	cpuMan.SetPC( cpuMan.GetPC() + 2 );

	ASSERT_MSG(static_cast<size_t>(OPMSN) < arr_size(instrTable), "Instruction Table Overflow!");
	

	// send the opcode most significant nibble to the first instruction table
	instrTable[OPMSN](cpuMan);
}




void op_0xxx(CpuManager& cpuMan)
{
	switch (cpuMan.GetOpcode())
	{
		case 0x00E0: // clear screen
			cpuMan.CleanGfx();
			break;

		case 0x00EE: // return from a subroutine ( unwind stack )
			cpuMan.SetSP( cpuMan.GetSP() - 1 );
			cpuMan.SetPC( cpuMan.GetStack( cpuMan.GetSP() ) );
			break;

		case 0x00FB: // 0x00FB* SuperChip: scrolls display 4 pixels right:
		{
			const auto res = cpuMan.GetRender()->GetResolution();
			for( int y = 0; y < res.y; ++y )
			{
				uint32_t* lineBeg = cpuMan.GetGfx() + res.x * y;
				std::memmove(lineBeg+4, lineBeg, sizeof(uint32_t) * (res.x-4));
				std::fill(lineBeg, lineBeg+4, 0);
			}
			break;
		}


		case 0x00FC: // 0x00FC* SuperChip: scrolls display 4 pixels left:
		{
			const auto res = cpuMan.GetRender()->GetResolution();
			for( int y = 0; y < res.y; ++y )
			{
				uint32_t* lineBeg = cpuMan.GetGfx() + res.x * y;
				uint32_t* lineEnd = lineBeg + res.x;
				std::memmove(lineBeg, lineBeg+4, sizeof(uint32_t) * (res.x-4));
				std::fill(lineEnd-4, lineEnd, 0); 
			}
			break;
		}

		case 0x00FD: // 0x00FD* SuperChip : exit CHIP interpreter
			// set error flag to exit
			cpuMan.SetFlags(Cpu::EXIT);
			break;



		case 0x00FE: // 0x00FE* SuperChip:  Disable extended screen mode
		{
			constexpr utility::Vec2i defaultRes(64,32);
			if (!cpuMan.GetRender()->SetResolution(defaultRes))
			{
				utility::LOGerr("Could not set extended resolution mode!");
				cpuMan.SetFlags(Cpu::EXIT);
			}

			cpuMan.SetGfxRes(defaultRes);
			cpuMan.GetRender()->SetBuffer(cpuMan.GetGfx());
			cpuMan.UnsetFlags(Cpu::EXTENDED_MODE);
			
			auto itr = std::find(std::begin(instrTable), std::end(instrTable), op_DXYN_ex);
			if( itr != std::end(instrTable))
				*itr = op_DXYN;

			break;

		}


		case 0x00FF: // 0x00FF* SuperChip: Enable extended screen mode 
		{
			constexpr utility::Vec2i extendedRes(128, 64);
			if(!cpuMan.GetRender()->SetResolution( extendedRes ))
			{
				utility::LOGerr("Could not set extended resolution mode!");
				cpuMan.SetFlags(Cpu::EXIT);
			}

			cpuMan.SetGfxRes(extendedRes);
			cpuMan.GetRender()->SetBuffer(cpuMan.GetGfx());
			cpuMan.SetFlags(Cpu::EXTENDED_MODE);
			
			auto itr = std::find(std::begin(instrTable), std::end(instrTable), op_DXYN);
			if( itr != std::end(instrTable))
				*itr = op_DXYN_ex;

			break;
		}



		default: // 0NNN or 00CN
		{
			if( (cpuMan.GetOpcode(0x00F0) >> 4) == 0xC )
			{
				// 00CN* SuperChip: Scroll display N lines down:
				const auto res = cpuMan.GetRender()->GetResolution();
				uint32_t* gfx = cpuMan.GetGfx();
				const int lines = N;			
			    for (int i = res.y - 1; i >= lines; --i)
			        memcpy(&gfx[i * res.x], &gfx[(i - lines) * res.x], sizeof(uint32_t) * res.x);

			    memset(&gfx[0], 0, sizeof(uint32_t)*(lines * res.x));

			}

			else
			{
				unknown_opcode(cpuMan);
			}

			break;
		}
	}
}



// 1NNN:  jumps to address NNN
void op_1NNN(CpuManager& cpuMan)
{
	cpuMan.SetPC( NNN );
}




// 2NNN: Calls subroutine at address NNN
void op_2NNN(CpuManager& cpuMan)
{
	cpuMan.GetStack(cpuMan.GetSP()) = cpuMan.GetPC();
	cpuMan.SetSP( cpuMan.GetSP() + 1 );
	cpuMan.SetPC( NNN );
}



// 3XNN: Skips the next instruction if VX equals NN
void op_3XNN(CpuManager& cpuMan)
{
	if (VX == NN)
		cpuMan.SetPC( cpuMan.GetPC() + 2 );
}



// 4XNN: Skips the next instruction if VX doesn't equal NN
void op_4XNN(CpuManager& cpuMan)
{
	if (VX != NN)
		cpuMan.SetPC( cpuMan.GetPC() + 2 );
}



// 5XY0: Skips the next instruction if VX equals VY
void op_5XY0(CpuManager& cpuMan)
{
	if (VX == VY)
		cpuMan.SetPC( cpuMan.GetPC() + 2 );
}


// 6XNN: store number NN in register VX
void op_6XNN(CpuManager& cpuMan)
{
	VX = static_cast<uint8_t>(NN);
}

// 7XNN: add the value NN to register VX
void op_7XNN(CpuManager& cpuMan)
{
	auto& vx = VX;
	vx = ((vx + NN) & 0xFF);
}




// 9XY0: skips the next instruction if VX doesn't equal VY
void op_9XY0(CpuManager& cpuMan)
{
	if (VX != VY)
		cpuMan.SetPC(cpuMan.GetPC() + 2 );
}



// ANNN: sets I to the address NNN
void op_ANNN(CpuManager& cpuMan)
{
	cpuMan.SetIndexRegister( NNN );
}



// BNNN: jumps to the address NNN plus V0
void op_BNNN(CpuManager& cpuMan)
{
	cpuMan.SetPC(  ((NNN + cpuMan.GetRegisters(0)) & 0xFFFF)  );
}



// CXNN: Sets VX to a bitwise operation AND ( & ) between NN and a random number
void op_CXNN(CpuManager& cpuMan)
{
	VX = ((std::rand() % 0xff) & NN);
}




// DXYN: DRAW INSTRUCTION
void op_DXYN(CpuManager& cpuMan)
{

	VF = 0;
	const auto& res = cpuMan.GetGfxRes();
	const auto resDec = res - 1;
	const auto vx = VX;
	const auto vy = VY;
	const int height = N;
	const uint8_t* _8bitRow =  cpuMan.GetMemory() + cpuMan.GetIndexRegister();

	for (int i = 0; i < height; ++i, ++_8bitRow)
	{
		for (int j = 0; j < 8; ++j)
		{
			auto& gfxPixel = cpuMan.GetGfx((vx + j) & resDec.x,  (vy + i) & resDec.y);
			const bool memoryBit = (*_8bitRow & (1 << (7 - j))) != 0;
			VF |= ((gfxPixel != 0) && memoryBit);
			gfxPixel ^= (memoryBit) ? 0xFFFFFFFF : 0;
		}
	}
}



// EXTENDED_MODE
void op_DXYN_ex(CpuManager& cpuMan)
{

	if(N)
	{
		op_DXYN(cpuMan);
		return; 
	}


	VF = 0;
	const auto vx = VX;
	const auto vy = VY;
	const auto& res = cpuMan.GetGfxRes();
	const auto resDec = res - 1;
	const uint8_t* _8bitRow = cpuMan.GetMemory() + cpuMan.GetIndexRegister();

	for(int i = 0; i < 16; ++i, ++_8bitRow)
	{
		for(int j = 0, bitmask = 0; j < 16; ++j, ++bitmask) 
		{
			if( bitmask == 8 )
			{
				bitmask = 0;
				++_8bitRow;
			}

			auto& gfxPixel = cpuMan.GetGfx((vx + j) & resDec.x,  (vy + i) & resDec.y);
			const bool memoryBit = (*_8bitRow & (1 << (7 - bitmask))) != 0;
			VF |= ((gfxPixel > 0) & memoryBit);
			gfxPixel ^= (memoryBit) ? 0xFFFFFFFF : 0;
		}
	}
}







// 2 instruction EX9E, EXA1
void op_EXxx(CpuManager& cpuMan)
{
	ASSERT_MSG(cpuMan.GetInput() != nullptr && cpuMan.GetInput()->IsInitialized(),
               "Cpu::Input, null or not initialized!");


	switch (N)
	{
		case 0xE: // EX9E  Skips the next instruction if the key stored in VX is pressed.
			if (cpuMan.GetInput()->IsKeyPressed((Key)VX))
				cpuMan.SetPC( cpuMan.GetPC() + 2 );
			break;


		case 0x1: // 0xEXA1  Skips the next instruction if the key stored in VX isn't pressed.
			if (!cpuMan.GetInput()->IsKeyPressed((Key)VX))
				cpuMan.SetPC( cpuMan.GetPC() + 2 );
			break;

		default: 
			unknown_opcode(cpuMan); 
			break;
	}



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

void op_8XYx(CpuManager& cpuMan)
{
	ASSERT_MSG(static_cast<size_t>(N) < arr_size(op_8XYx_Table),
		"op_8XYx_Table Overflow!");

	// call it
	op_8XYx_Table[N](cpuMan);

}




// 8XY0: store the value of register VY in register VX
void op_8XY0(CpuManager& cpuMan)
{
	VX = VY;
}






// 8XY1: set VX to VX | VY
void op_8XY1(CpuManager& cpuMan)
{
	auto& vx = VX;
	vx = (VY | vx);
}







// 8XY2: sets VX to VX and VY
void op_8XY2(CpuManager& cpuMan)
{
	auto& vx = VX;
	vx = (VY & vx);
}






// 8XY3: sets VX to VX xor VY
void op_8XY3(CpuManager& cpuMan)
{
	auto& vx = VX;
	vx = ((VY ^ vx) & 0xFF);
}






// 8XY4: Adds VY to VX . VF is set to 1 when theres a carry, and to 0 when there isn't
void op_8XY4(CpuManager& cpuMan)
{
	auto& vx = VX;
	uint16_t result = vx + VY; // compute sum
	VF = (result & 0xff00) != 0 ? 1 : 0; // check carry
	vx = (result & 0xff);
}







// 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY5(CpuManager& cpuMan)
{
	auto const vy = VY;
	auto& vx = VX;
	VF = vx > vy; // checking if theres is a borrow
	vx -= vy;
}








// 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
void op_8XY6(CpuManager& cpuMan)
{
	auto& vx = VX;
	VF = vx & 0x1; // check the least significant bit
	vx >>= 1;
}







// 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void op_8XY7(CpuManager& cpuMan)
{
	const auto vy = VY;
	auto &vx = VX; 
	VF = vy > vx; // check borrow ( VY > VX )
	vx = vy - vx;
}







// 8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
void op_8XYE(CpuManager& cpuMan)
{
	auto& vx = VX;
	VF = (vx & 0x80) >> 7;  // check the most significant bit
	vx = (vx << 1) & 0xFF;
}


/******** OP_8XYEx END *********/








/******** OP_FXxx START *********/

// FXxxx subtable start
static InstrTable op_FXxx_Table[] =
{
	op_FX30, unknown_opcode, unknown_opcode,
	op_FX33, unknown_opcode, op_FXx5, unknown_opcode,
	op_FX07, op_FX18, op_FX29, op_FX0A, unknown_opcode,
	unknown_opcode, unknown_opcode, op_FX1E
};




void op_FXxx(CpuManager& cpuMan) // 9 instructions.
{
	ASSERT_MSG(static_cast<size_t>(N) < arr_size(op_FXxx_Table),
		"op_FXxx_Table overflow...");

	op_FXxx_Table[N](cpuMan);
}


// Set I to the Hi Res font corresponding the digit in VX
void op_FX30(CpuManager& cpuMan)
{
	cpuMan.SetIndexRegister( arr_size(fonts::chip8DefaultFont) + (VX*10) );
}



// FX07   Sets VX to the value of the delay timer.
void op_FX07(CpuManager& cpuMan)
{
	VX = cpuMan.GetDelayTimer();
}




// FX0A   A key press is awaited, and then stored in VX.
void op_FX0A(CpuManager& cpuMan)
{
	ASSERT_MSG(cpuMan.GetInput() != nullptr && cpuMan.GetInput()->IsInitialized(),
               "Cpu::input, null or not initialized!");

	VX = static_cast<uint8_t>(cpuMan.GetInput()->WaitKeyPress());
}





void op_FXx5(CpuManager& cpuMan)
{

	switch (NN)
	{
		case 0x15: // FX15  Sets the delay timer to VX.
			cpuMan.SetDelayTimer( VX );
			break;


		case 0x55: //FX55  Stores V0 to VX in memory starting at address I
			ASSERT_MSG(static_cast<size_t>(X+1) < (cpuMan.GetMemorySize() - cpuMan.GetIndexRegister()),
				"memory overflow");

			std::copy_n(cpuMan.GetRegisters(), X+1, &cpuMan.GetMemory(cpuMan.GetIndexRegister()));
			break;

		case 0x65: //FX65  Fills V0 to VX with values from memory starting at address I.
			ASSERT_MSG(static_cast<size_t>(X+1) < cpuMan.GetRegistersSize(),
				"registers overflow");

			std::copy_n(&cpuMan.GetMemory(cpuMan.GetIndexRegister()), X+1, cpuMan.GetRegisters());
			break;

		case 0x75: // 0xFX75* SuperChip: Store V0...VX in RPL user flags ( X <= 7 )
		{
			constexpr auto rplOffset = arr_size(fonts::chip8DefaultFont) + arr_size(fonts::chip8HiResFont);
			auto* const rpl = cpuMan.GetMemory() + rplOffset;
			std::copy_n(cpuMan.GetRegisters(),  VX, rpl);
//			utility::LOGerr("opcode 0xFX75 not implemented.");
			break;

		}
		case 0x85: // 0xFX85* SuperChip: Read V0...VX from RPL user flags ( X <= 7 )
		{
			constexpr auto rplOffset = arr_size(fonts::chip8DefaultFont) + arr_size(fonts::chip8HiResFont);
			const auto* const rpl = cpuMan.GetMemory() + rplOffset;
			std::copy_n(rpl, VX, cpuMan.GetRegisters());
//			utility::LOGerr("opcode 0xFX85 not implemented.");
			break;
		}
		default: 
			unknown_opcode(cpuMan); 
			break;


	}
}





// FX18   Sets the sound timer to VX.
void op_FX18(CpuManager& cpuMan)
{
	ASSERT_MSG(cpuMan.GetSound() != nullptr && cpuMan.GetSound()->IsInitialized(),
               "Cpu::sound, null or not initialized");

	cpuMan.SetSoundTimer(VX);

	if (cpuMan.GetSoundTimer() > 0) {
		cpuMan.GetSound()->Play(cpuMan.GetSoundTimer());
	}
	else {
		if (cpuMan.GetSound()->IsPlaying())
			cpuMan.GetSound()->Stop();
	}
}




// FX1E   Adds VX to I.
void op_FX1E(CpuManager& cpuMan)
{
	cpuMan.SetIndexRegister( ((cpuMan.GetIndexRegister() + VX) & 0xFFFF) );
}



// FX29  Sets I to the location of the sprite for the character in VX. 
void op_FX29(CpuManager& cpuMan)
{
	// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
	cpuMan.SetIndexRegister( VX * 5 );
}




// BCD: Stores the Binary-coded decimal representation of VX, with the most significant 
// of three digits at the address in I, the middle digit at I plus 1, 
// and the least significant digit at I plus 2. 
// (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, 
//  the tens digit at location I+1, and the ones digit at location I+2.)
void op_FX33(CpuManager& cpuMan)
{
	ASSERT_MSG(cpuMan.GetMemorySize() > (cpuMan.GetIndexRegister() + 2),
		"Cpu::I + 2 overflows Cpu::memory!");

	auto* memory = cpuMan.GetMemory() + cpuMan.GetIndexRegister();
	const uint8_t vx = VX;
	memory[2] = vx % 10;
	memory[1] = (vx / 10) % 10;
	memory[0] = (vx / 100);

}









/******** OP_FXxx END *********/









}}
