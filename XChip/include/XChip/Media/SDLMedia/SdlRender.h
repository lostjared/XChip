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
	bool Initialize(const int width, const int height) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	const char* GetWindowName() const noexcept override;
	const uint32_t* GetBuffer() const noexcept override;
	utility::Color GetColorFilter() const noexcept override;
	utility::Resolution GetResolution() const noexcept override;



	void SetWindowName(const char* name) noexcept override;
	bool SetFullScreen(const bool val) noexcept override;
	bool SetResolution(const utility::Resolution& res) noexcept override;
	bool SetColorFilter(const utility::Color& color) noexcept override;
	void SetScroll(const int *x, const int *y, bool lines = false) noexcept override;



	void SetBuffer(const uint32_t* gfx) noexcept override;
	void DrawBuffer() noexcept override;
	void HideWindow() noexcept override;
	void ShowWindow() noexcept override;
	bool UpdateEvents() noexcept override;

	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept override;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept override;

private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _rend = nullptr;
	SDL_Texture* _texture = nullptr;
	SDL_Rect* _camera     = nullptr;
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
