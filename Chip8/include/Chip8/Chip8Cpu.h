#ifndef __CHIP8CPU_H__
#define __CHIP8CPU_H__
#include <cstdint>

class iRenderer;
class iInput;

struct Chip8Cpu
{
	/* basic Chip8Emu registers names */
	enum { V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VF };
	
	uint16_t opcode;
	uint16_t pc;
	uint16_t I;                     /* index register */
	uint8_t sp;

	uint8_t* memory    = nullptr;
	uint8_t* registers = nullptr;
	uint16_t* stack    = nullptr;
	uint32_t* gfx      = nullptr;  /* graphics array  */
	iRenderer* render  = nullptr;
	iInput* input      = nullptr;
	bool exitFlag      = false;
	
	uint8_t delayTimer;
	uint8_t soundTimer;

	

	/* prevent mess */
	Chip8Cpu() noexcept = default;
	Chip8Cpu(const Chip8Cpu&) = delete;
	const Chip8Cpu& operator=(const Chip8Cpu&) = delete;
};




#endif
