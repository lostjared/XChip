#ifndef _XCHIP_SDLINPUT_H_
#define _XCHIP_SDLINPUT_H_
#include <vector>
#include <XChip/Interfaces/iInput.h>
#include "SdlMedia.h"

 
namespace xchip {

	
class SdlInput final : private SdlMedia, public iInput
{
	using SDL_Scancode = int;
	using KeyPair = std::pair<Key, SDL_Scancode>;
public:
	SdlInput();
	~SdlInput();
	
	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override;
	bool IsKeyPressed(const Key key) const override;
	Key GetPressedKey() const override;

	bool UpdateKeys() override;
	Key WaitKeyPress() override;

	void SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) override;
	void SetResetKeyCallback(const void* arg, ResetKeyCallback callback) override;
	void SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) override;

private:
	std::vector<KeyPair> _keyPairs;
	const unsigned char* _keyboardState = nullptr;
	WaitKeyCallback _waitClbk = nullptr;
	ResetKeyCallback _resetClbk = nullptr;
	EscapeKeyCallback _escapeClbk = nullptr;
	const void* _waitClbkArg;
	const void* _resetClbkArg;
	const void* _escapeClbkArg;
	bool _initialized = false;

};






inline bool SdlInput::IsInitialized() const { return _initialized; }


}














#endif
