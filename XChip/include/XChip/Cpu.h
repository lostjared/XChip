#ifndef __XCHIP_CPU_H__
#define __XCHIP_CPU_H__
#include <cstdint>


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

	uint16_t opcode;
	size_t I;
	size_t sp;
	size_t pc;
	uint8_t delayTimer;
	uint8_t soundTimer;
};







}









#endif
