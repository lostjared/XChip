#ifndef _XCHIP_SDLRENDER_H_
#define _XCHIP_SDLRENDER_H_
#include <XChip/Media/iRender.h>
#include "SdlSystem.h"
 
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;


namespace xchip {


class SdlRender final : public iRender, private SdlSystem
{

public:
	SdlRender() noexcept;
	~SdlRender();
	bool Initialize(const int width, const int height) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	const uint32_t* GetBuffer() const noexcept override;
	utility::Color GetColorFilter() const noexcept override;


	bool SetResolution(const int w, const int h) noexcept override;
	bool SetColorFilter(const utility::Color& color) noexcept override;
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
