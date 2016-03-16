#ifndef CHIP8_H
#define CHIP8_H
#include <Chip8/Chip8Manager.h>
#include <Chip8/Utility/Timer.h>


class Chip8Emu : private Chip8CpuManager
{
public:
	Chip8Emu() noexcept = default;
	~Chip8Emu();

	bool Initialize() noexcept;
	void Dispose() noexcept;

	int GetInstrPerSec() const;
	int GetFramesPerSec() const;
	void HaltForNextFlag() const;

	void SetInstrPerSec(const int instr);
	void SetFramesPerSec(const int frames);
	
	void UpdateSystems();
	void Draw();
	void ExecuteNextOpcode();
	void CleanFlags();

	bool InitRender(const int w, const int h);
	bool InitInput();

	inline bool GetInstrFlag() const { return m_instrFlag; }
	inline bool GetDrawFlag() const { return m_drawFlag; }
	inline void SetInstrFlag(bool val) { m_instrFlag = val; }
	inline void SetDrawFlag(bool val) { m_drawFlag = val; }


	using Chip8CpuManager::GetRenderer;
	using Chip8CpuManager::GetInput;
	using Chip8CpuManager::SetRenderer;
	using Chip8CpuManager::SetInput;
	using Chip8CpuManager::SwapRender;
	using Chip8CpuManager::SwapInput;

private:
	bool m_instrFlag = false;
	bool m_drawFlag = false;
	bool m_needToDispose = false;
	

	struct {
		Timer instr;
		Timer frame;
	} m_clocks;
	
};

#endif // CHIP8_H
