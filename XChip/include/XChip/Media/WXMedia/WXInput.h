#ifndef _XCHIP_MEDIA_WXINPUT_H_
#define _XCHIP_MEDIA_WXINPUT_H_
#include <vector>
#include <XChip/Media/iInput.h>

namespace xchip {

class WXInput final : public iInput
{
public:
	using KeyPair = std::pair<Key, wxKeyCode>;
	WXInput() noexcept;
	~WXInput();
	
	bool Initialize() noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	bool IsKeyPressed(const Key key) const noexcept override;
	Key GetPressedKey() const noexcept override;
	
	bool UpdateKeys() noexcept override;
	Key WaitKeyPress() noexcept override;
	
	void SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept override;
	void SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept override;
	void SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept override;
	
private:
	WaitKeyCallback _waitClbk = nullptr;
	ResetKeyCallback _resetClbk = nullptr;
	EscapeKeyCallback _escapeClbk = nullptr;
	const void* _waitClbkArg;
	const void* _resetClbkArg;
	const void* _escapeClbkArg;
	bool _initialized = false;
	std::vector<KeyPair> _keyPairs;
	

};
	










}

#endif