#ifndef _XCHIP_EMULATOR_H_
#define _XCHIP_EMULATOR_H_
#include "CpuManager.h"
#include "Utility/Timer.h"



namespace xchip {


class Emulator
{
public:
	Emulator() noexcept;
	~Emulator();
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	bool Initialize(iRender* render, iInput* input, iSound* sound) noexcept;
	void Dispose() noexcept;

	bool GetInstrFlag() const;
	bool GetDrawFlag() const;
	bool GetExitFlag() const;
	void HaltForNextFlag() const;

	void UpdateTimers();
	void UpdateSystems();
	void ExecuteInstr();
	void Draw();
	void Reset();

	void SetInstrPerSec(const unsigned short value);
	void SetFramesPerSec(const unsigned short value);
	bool LoadRom(const char* fname);
	iRender* GetRender();
	iInput* GetInput();
	iSound* GetSound();
	iRender* SwapRender(iRender* rend);
	iInput* SwapRender(iInput* input);
	iSound* SwapSound(iSound* sound);

private:
	bool InitMedia(iRender* rend, iInput* input, iSound* sound);
	bool InitRender();
	bool InitInput();
	bool InitSound();

	CpuManager _manager;
	utility::Timer _instrTimer;
	utility::Timer _frameTimer;
	bool _instrf = false;
	bool _drawf  = false;
	bool _exitf  = true;
	bool _initialized = false;

};


inline bool Emulator::GetInstrFlag() const { return _instrf; }
inline bool Emulator::GetDrawFlag() const { return _drawf; }
inline bool Emulator::GetExitFlag() const { return _exitf; }

inline void Emulator::SetInstrPerSec(const unsigned short value) { 
	_instrTimer.SetTargetTime(utility::literals::operator""_hz(value));
}
inline void Emulator::SetFramesPerSec(const unsigned short value) { 
	_frameTimer.SetTargetTime(utility::literals::operator""_hz(value));
}

inline bool Emulator::LoadRom(const char* fname) { return _manager.LoadRom(fname); }
inline iRender* Emulator::GetRender() { return _manager.GetRender(); }
inline iInput* Emulator::GetInput() { return _manager.GetInput(); }
inline iSound* Emulator::GetSound() { return _manager.GetSound(); }
inline iRender* Emulator::SwapRender(iRender* rend) { return _manager.SwapRender(rend); }
inline iInput* Emulator::SwapRender(iInput* input) { return _manager.SwapInput(input); }
inline iSound* Emulator::SwapSound(iSound* sound) { return _manager.SwapSound(sound); }


}









#endif
