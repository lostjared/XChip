#ifndef CHIP8_H
#define CHIP8_H
#include <Chip8/Chip8Manager.h>



class Chip8 : private Chip8Manager
{
public:
	bool Initialize() noexcept;
	void Dispose() noexcept;

	inline bool GetInstrFlag() const;
	inline bool GetDrawFlag() const;
	inline bool GetExitFlag() const;
	inline bool SetInstrFlag(bool val);
	inline bool SetDrawFlag(bool val);
	inline bool SetExitFlag(bool val);

private:
	bool m_instrFlag;
	bool m_drawFlag;
	bool m_exitFlag;
};

inline bool Chip8::GetInstrFlag() const {
	return m_instrFlag;
}

inline bool Chip8::GetDrawFlag() const {
	return m_drawFlag;
}

inline bool Chip8::GetExitFlag() const {
	return m_exitFlag;
}


inline bool Chip8::SetInstrFlag(bool val) {
	m_instrFlag = val;
}
inline bool Chip8::SetDrawFlag(bool val) {
	m_drawFlag = val;
}
inline bool Chip8::SetExitFlag(bool val) {
	m_exitFlag = val;
}

#endif // CHIP8_H
