#include <memory>
#include <iostream>
#include <Chip8/Chip8Emu.h>


int main(int argc, char **argv)
{
	
	Chip8Emu emu;

	/*  debug */
	std::atexit([]() {std::cin.ignore(); });
	return 0;
}

