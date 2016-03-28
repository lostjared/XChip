#ifndef _XCHIP_SDLRENDER_H_
#define _XCHIP_SDLRENDER_H_
#include <XChip/Interfaces/iRender.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;


namespace xchip {

class SdlRender final : public iRender
{

public:
	SdlRender();
	~SdlRender();

	bool Initialize(const int width, const int height) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const override { return m_initialized; }
	void SetBuffer(const uint32_t *gfx) override;
	void DrawBuffer() override;
	bool UpdateEvents() override;

	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) override;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callback) override;
private:
	SDL_Window* m_window;
	SDL_Renderer* m_rend;
	SDL_Texture* m_texture;
	const uint32_t* m_buffer;
	int m_pitch;
	WinCloseCallback m_closeClbk;
	WinResizeCallback m_resizeClbk;
	const void* m_closeClbkArg;
	const void* m_resizeClbkArg;
	bool m_initialized;



};




}
#endif
