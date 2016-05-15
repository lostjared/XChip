#ifndef __XCHIP_CPU_H__
#define __XCHIP_CPU_H__
#include <XChip/Utility/StdintDef.h>
 

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
	
	enum Flags : uint32_t { EXIT = 0x0000001, EXTENDED_MODE = 0x00000002 };
};







}









#endif
