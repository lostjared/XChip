#ifndef __WXRENDER__H__
#define __WXRENDER__H__

#include <XChip/Media/iRender.h>

namespace xchip {

class WXRender final : public iRender
{
	
public:
	WXRender() noexcept;
	~WXRender();
	bool Initialize(const int width, const int height) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	utility::Color GetColorFilter() const noexcept override;
	bool SetColorFilter(const utility::Color& color) noexcept override;
	void SetBuffer(const uint32_t* gfx) noexcept override;
	void DrawBuffer() noexcept override;
	void HideWindow() noexcept override;
	void ShowWindow() noexcept override;
	bool UpdateEvents() noexcept override;
	
	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept override;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept override;
	
private:
	bool _initialized = false;
};

inline bool WXRender::IsInitialized() const noexcept { return _initialized; }
inline void WXRender::SetBuffer(const uint32_t* gfx) noexcept { /*_buffer = gfx;*/ }

}

#endif

