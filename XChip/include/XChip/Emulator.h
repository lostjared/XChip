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
	bool Initialize(iRender* const render, iInput* const input) noexcept;
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

	void SetInstrPerSec(const unsigned short value);
	void SetFramesPerSec(const unsigned short value);

	bool LoadRom(const char* fname) { return _manager.LoadRom(fname); }
	bool SetAndInitRender(iRender* rend) { return this->InitRender(rend); }
	bool SetAndInitInput(iInput* input) { return this->InitInput(input); }
	iRender* GetRender() { return _manager.GetRender(); }
	iInput* GetInput() { return _manager.GetInput(); }
	iRender* SwapRender(iRender* rend) { return _manager.SwapRender(rend); }
	iInput* SwapRender(iInput* input) { return _manager.SwapInput(input); }
	
private:
	bool InitRender(iRender* const rend);
	bool InitInput(iInput* const input);

private:
	CpuManager _manager;
	utility::Timer _instrTimer;
	utility::Timer _frameTimer;
	bool _instrf;
	bool _drawf;
	bool _exitf;
	bool _initialized;


};




}









#endif
