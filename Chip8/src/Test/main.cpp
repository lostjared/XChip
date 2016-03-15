#include <memory>
#include <iostream>
#include <Chip8/Chip8.h>


int main(int argc, char **argv)
{
	
	Chip8 chip8;

	/*  debug */
	std::atexit([]() {std::cin.ignore(); });
	return 0;
}

