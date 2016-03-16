#include <memory>
#include <iostream>
#include <Chip8/Chip8Manager.h>


int main(int argc, char **argv)
{
	
	Chip8Manager chpman;

	/*  debug */
	std::atexit([]() {std::cin.ignore(); });
	return 0;
}

