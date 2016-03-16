#ifndef __CHIP8_EMU_H__
#define __CHIP8_EMU_H__
#include <Chip8/Chip8CpuManager.h>
#include <Chip8/Utility/Timer.h>


class Chip8Emu : public Chip8CpuManager
{
	static constexpr std::size_t CHIP8_DEFAULT_WIDHT = 64;
	static constexpr std::size_t CHIP8_DEFAULT_HEIGHT = 32;
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

	inline bool GetInstrFlag() const { return m_instrFlag; }
	inline bool GetDrawFlag() const { return m_drawFlag; }
	inline void SetInstrFlag(bool val) { m_instrFlag = val; }
	inline void SetDrawFlag(bool val) { m_drawFlag = val; }


private:
	bool m_instrFlag = false;
	bool m_drawFlag = false;
	bool m_needToDispose = false;
	

	struct {
		Timer instr;
		Timer frame;
		int instrPerSec;
		int framesPerSec;
	} m_clocks;
	
};

#endif // CHIP8_H
