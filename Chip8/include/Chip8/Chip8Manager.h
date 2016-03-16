#ifndef __CHIP8MANAGER_H__
#define __CHIP8MANAGER_H__
#include <cstddef>
#include <cstdint>

struct Chip8Cpu;
class iRenderer;
class iInput;

class Chip8Manager
{
public:
	Chip8Manager() noexcept;
	~Chip8Manager();
	bool Initialize() noexcept;
	void Dispose() noexcept;


	bool SetMemory(const std::size_t size);
	bool SetRegisters(const std::size_t size);
	bool SetStack(const std::size_t size);
	bool SetGfx(const std::size_t size);
	void LoadFont(const uint8_t* font, const std::size_t size);
	bool LoadRom(const char* fileName);

	iRenderer* GetRenderer();
	iInput* GetInput();

	void SetRenderer(iRenderer* rend);
	void SetInput(iInput* input);
	
	iRenderer* SwapRender(iRenderer* rend);
	iInput* SwapInput(iInput* input);

protected:
	Chip8Cpu* m_cpu;
	const char* m_rom;
	std::size_t m_memorysz;
	std::size_t m_romsz;

};











#endif