#ifndef _XCHIP_SDLRENDER_H_
#define _XCHIP_SDLRENDER_H_
#include <XChip/Interfaces/iRender.h>
#include "SdlMedia.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace xchip {


class SdlRender final : private SdlMedia, public iRender
{

public:
	SdlRender();
	~SdlRender();

	bool Initialize(const int width, const int height) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const override { return _initialized; }
	void SetBuffer(const uint32_t* gfx) override { _buffer = gfx; }
	void DrawBuffer() override;
	bool UpdateEvents() override;

	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) override;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callback) override;

private:
	SDL_Window* _window = nullptr;
	SDL_Renderer* _rend = nullptr;
	SDL_Texture* _texture = nullptr;
	const uint32_t* _buffer = nullptr;
	int _pitch;
	WinCloseCallback _closeClbk = nullptr;
	WinResizeCallback _resizeClbk = nullptr;
	const void* _closeClbkArg;
	const void* _resizeClbkArg;
	bool _initialized = false;
};




}
#endif
