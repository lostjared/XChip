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

	bool Initialize(iRender* const render, iInput* const input, iSound* const sound) noexcept;
	void Dispose() noexcept;

	bool GetInstrFlag() const { return _instrf; }
	bool GetDrawFlag() const { return _drawf; }
	bool GetExitFlag() const { return _exitf; }
	void HaltForNextFlag() const;

	void UpdateTimers();
	void UpdateSystems();
	void ExecuteInstr();
	void Draw();
	void Reset();

	void SetInstrPerSec(const unsigned short value) {
		_instrTimer.SetTargetTime(utility::literals::operator""_hz(value));
	}
	void SetFramesPerSec(const unsigned short value) {
		_frameTimer.SetTargetTime(utility::literals::operator""_hz(value));
	}

	bool LoadRom(const char* fname) { return _manager.LoadRom(fname); }
	iRender* GetRender() { return _manager.GetRender(); }
	iInput* GetInput() { return _manager.GetInput(); }
	iSound* GetSound() { return _manager.GetSound(); }
	iRender* SwapRender(iRender* rend) { return _manager.SwapRender(rend); }
	iInput* SwapRender(iInput* input) { return _manager.SwapInput(input); }
	iSound* SwapSound(iSound* sound) { return _manager.SwapSound(sound); }

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




}









#endif
