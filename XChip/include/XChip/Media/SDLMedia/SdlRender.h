#ifndef _XCHIP_SDLRENDER_H_
#define _XCHIP_SDLRENDER_H_
#include <XChip/Media/iRender.h>
#include "SdlSystem.h"
 
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

namespace xchip {


class SdlRender final : public iRender, private SdlSystem
{

public:
	SdlRender() noexcept;
	~SdlRender();
	bool Initialize(const utility::Vec2i& winSize, const utility::Vec2i& res) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	const char* GetWindowName() const noexcept override;
	const uint32_t* GetBuffer() const noexcept override;
	utility::Color GetColorFilter() const noexcept override;
	utility::Vec2i GetResolution() const noexcept override;
	utility::Vec2i GetWindowSize() const noexcept override;
	
	void SetWindowName(const char* name) noexcept override;
	void SetBuffer(const uint32_t* gfx) noexcept override;
	bool SetColorFilter(const utility::Color& color) noexcept override;
	bool SetResolution(const utility::Vec2i& res) noexcept override;
	void SetWindowSize(const utility::Vec2i& size) noexcept override;
	bool SetFullScreen(const bool val) noexcept override;
	bool UpdateEvents() noexcept override;
	void DrawBuffer() noexcept override;
	void HideWindow() noexcept override;
	void ShowWindow() noexcept override;

	void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept override;
	void SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept override;

private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _rend = nullptr;
	SDL_Texture* _texture = nullptr;
	const uint32_t* _buffer = nullptr;
	WinCloseCallback _closeClbk = nullptr;
	WinResizeCallback _resizeClbk = nullptr;
	const void* _closeClbkArg;
	const void* _resizeClbkArg;
	int _pitch;
	bool _initialized = false;
};








inline bool SdlRender::IsInitialized() const noexcept { return _initialized; }
inline const uint32_t* SdlRender::GetBuffer() const noexcept { return _buffer; }

inline void SdlRender::SetBuffer(const uint32_t* gfx) noexcept { _buffer = gfx; }












}
#endif
