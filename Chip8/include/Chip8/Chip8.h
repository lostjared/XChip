#ifndef CHIP8_H
#define CHIP8_H
#include <Chip8/Chip8Manager.h>
#include <Chip8/Utility/Timer.h>


class Chip8 : private Chip8Manager
{
public:
	Chip8() noexcept = default;
	bool Initialize() noexcept;
	void Dispose() noexcept;

	void HaltForNextFlag() const;
	void Draw();
	void ExecuteNextOpcode();

	inline bool GetInstrFlag() const { return m_instrFlag; }
	inline bool GetDrawFlag() const { return m_drawFlag; }
	inline void SetInstrFlag(bool val) { m_instrFlag = val; }
	inline void SetDrawFlag(bool val) { m_drawFlag = val; }

private:
	bool m_instrFlag = false;
	bool m_drawFlag  = false;

	struct {
		Timer instr;
		Timer frame;
	} m_clocks;
	
};

#endif // CHIP8_H
