#ifndef __CHIP8_CPU_MANAGER_H__
#define __CHIP8_CPU_MANAGER_H__
#include <cstddef>
#include <cstdint>
#include <Chip8/Chip8Cpu.h>

class iRenderer;
class iInput;

class Chip8CpuManager
{
public:
	static constexpr std::size_t MEMORY_MIN = 0x1000;
	static constexpr std::size_t STACK_MIN = 16;
	static constexpr std::size_t REGISTERS_MIN = 16;
	static constexpr std::size_t GFX_MIN = 64 * 32;

public:
	Chip8CpuManager() noexcept;
	~Chip8CpuManager();
	bool Initialize() noexcept;
	void Dispose() noexcept;


	bool SetMemory(const std::size_t size = MEMORY_MIN);
	bool SetRegisters(const std::size_t size = REGISTERS_MIN);
	bool SetStack(const std::size_t size = STACK_MIN);
	bool SetGfx(const std::size_t size = GFX_MIN);
	void SetFont(const uint8_t* font, const std::size_t size);
	bool LoadRom(const char* fileName);


	bool InitRender(const int w, const int h);
	bool InitInput();

	iRenderer* GetRenderer() { return m_cpu->render; }
	iInput* GetInput() { return m_cpu->input; }

	void SetRenderer(iRenderer* rend) { m_cpu->render = rend; }
	void SetInput(iInput* input) { m_cpu->input = input; }

	iRenderer* SwapRender(iRenderer* rend);
	iInput* SwapInput(iInput* input);


protected:
	Chip8Cpu* m_cpu;
	const char* m_romName;
	std::size_t m_romsz;

};











#endif