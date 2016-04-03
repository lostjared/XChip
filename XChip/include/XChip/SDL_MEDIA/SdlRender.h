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
	
	bool IsInitialized() const override { return m_initialized; }
	void SetBuffer(const uint32_t* gfx) override { m_buffer = gfx; }
	void DrawBuffer() override;
	bool UpdateEvents() override;

	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) override;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callback) override;
private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_rend = nullptr;
	SDL_Texture* m_texture = nullptr;
	const uint32_t* m_buffer = nullptr;
	int m_pitch;
	WinCloseCallback m_closeClbk = nullptr;
	WinResizeCallback m_resizeClbk = nullptr;
	const void* m_closeClbkArg;
	const void* m_resizeClbkArg;
	bool m_initialized = false;
};




}
#endif
