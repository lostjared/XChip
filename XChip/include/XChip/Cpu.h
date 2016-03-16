#ifndef __XCHIP_CPU_H__
#define __XCHIP_CPU_H__
#include <cstdint>
#include "XChip.h"
XCHIP_NAMESPACE




struct Cpu
{
	uint16_t opcode;
	uint16_t I;
	uint16_t pc;
	uint16_t sp;
	uint8_t* memory;
	uint8_t* registers;
	uint16_t* stack;
	uint32_t* gfx;

	uint8_t delayTimer;
	uint8_t soundTimer;
};











NAMESPACE_END









#endif
