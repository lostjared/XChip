#ifndef _XCHIP_EMULATOR_H_
#define _XCHIP_EMULATOR_H_
#include <memory>
#include "CpuManager.h"
#include "Utility/Timer.h"



namespace xchip {
using UniqueRender = std::unique_ptr<iRender>;
using UniqueInput = std::unique_ptr<iInput>;
using UniqueSound = std::unique_ptr<iSound>;
using SharedRender = std::shared_ptr<iRender>;
using SharedInput = std::shared_ptr<iInput>;
using SharedSound = std::shared_ptr<iSound>;

class Emulator
{
public:
	Emulator() noexcept;
	~Emulator();
	Emulator(const Emulator&) = delete;
	Emulator& operator=(const Emulator&) = delete;

	bool Initialize(UniqueRender render, UniqueInput input, UniqueSound sound) noexcept;

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

	bool SetRender(iRender* rend);
	bool SetInput(iInput* input);
	bool SetSound(iSound* sound);

	const SharedRender GetRender();
	const SharedInput GetInput();
	const SharedSound GetSound();
	
	UniqueRender SwapRender(UniqueRender rend = nullptr);
	UniqueInput SwapInput(UniqueInput input = nullptr);
	UniqueSound SwapSound(UniqueSound sound = nullptr);

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


inline bool Emulator::LoadRom(const char* fname) { _manager.SetPC(0x200); return _manager.LoadRom(fname, 0x200); }
inline bool Emulator::SetRender(iRender* rend) { _manager.SetRender(rend); return InitRender(); }
inline bool Emulator::SetInput(iInput* input) { _manager.SetInput(input); return InitInput(); }
inline bool Emulator::SetSound(iSound* sound) { _manager.SetSound(sound); return InitSound(); }

inline const SharedRender Emulator::GetRender() { return SharedRender(_manager.GetRender(), [](void*) {}); }
inline const SharedInput Emulator::GetInput() { return SharedInput(_manager.GetInput(), [](void*) {}); }
inline const SharedSound Emulator::GetSound() { return SharedSound(_manager.GetSound(), [](void*) {}); }












}
#endif
