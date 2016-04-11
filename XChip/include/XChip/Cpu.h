#ifndef __XCHIP_CPU_H__
#define __XCHIP_CPU_H__
#include "Utility/StdintDef.h"
 

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
	uint16_t opcode;
	uint8_t delayTimer;
	uint8_t soundTimer;
};







}









#endif
