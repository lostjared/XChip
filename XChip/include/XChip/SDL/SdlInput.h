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
	SdlInput(const SdlInput&) = delete;
	SdlInput& operator=(const SdlInput&) = delete;
	~SdlInput();

	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override { return m_initialized;  }
	bool IsKeyPressed(const Key key) const override;
	Key GetPressedKey() const override;

	bool UpdateKeys() override;
	Key WaitKeyPress() override;

	void SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) override;
	void SetResetKeyCallback(const void* arg, ResetKeyCallback callback) override;
	void SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) override;

private:
	const unsigned char* m_keyboardState;
	std::vector<KeyPair> m_keyPairs;
	WaitKeyCallback m_waitClbk;
	ResetKeyCallback m_resetClbk;
	EscapeKeyCallback m_escapeClbk;
	const void* m_waitClbkArg;
	const void* m_resetClbkArg;
	const void* m_escapeClbkArg;
	bool m_initialized;

};









}














#endif
