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
	uint16_t* stack;
	uint32_t* gfx;

	iRender* render;
	iInput* input;
	iSound* sound;

	uint16_t opcode;
	uint16_t I;
	uint16_t sp;
	uint16_t pc;
	uint8_t delayTimer;
	uint8_t soundTimer;
};







}









#endif
