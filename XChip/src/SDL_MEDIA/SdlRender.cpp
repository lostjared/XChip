#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include <XChip/SDL_MEDIA/SdlRender.h>
#include <XChip/Utility/Log.h>

namespace xchip {
using namespace utility;



SdlRender::SdlRender()
	: SdlMedia(System::Render)

{
	LOG("Creating SdlRenderer object...");
}


SdlRender::~SdlRender()
{
	LOG("Destroying SdlRenderer object...");
	if (m_window != nullptr)
		this->Dispose();
}





bool SdlRender::Initialize(const int width, const int height) noexcept
{
	using namespace utility::literals;

	if (m_window != nullptr)
		this->Dispose();

	else if (!this->InitSubSystem())
		return false;

	m_pitch = width * 4;


	m_window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width * 4, height * 6, SDL_WINDOW_RESIZABLE);


	if (m_window == nullptr) 
	{
		LOGerr("Couldn't allocate SDL_Window. Error: "_s + SDL_GetError());
		return false;
	}

	m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);


	if (m_rend == nullptr)
	{
		LOGerr("Couldn't allocate SDL_Renderer. Error: "_s + SDL_GetError());
		SDL_DestroyWindow(m_window);
		return false;
	}


	m_texture = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);


	if (m_texture == nullptr)
	{
		LOGerr("Couldn't allocate SDL_Texture. Error: "_s + SDL_GetError());
		SDL_DestroyRenderer(m_rend);
		SDL_DestroyWindow(m_window);
		return false;
	}

	SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);
	SDL_RenderClear(m_rend);
	SDL_RenderPresent(m_rend);

	m_initialized = true;
	return true;
}


void SdlRender::Dispose() noexcept
{
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_rend);
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	m_buffer = nullptr;
	m_closeClbk = nullptr;
	m_resizeClbk = nullptr;
	m_initialized = false;
}





bool SdlRender::UpdateEvents()
{
	SdlMedia::UpdateEvents();
	
	if (GetEvent().type == SDL_WINDOWEVENT)
	{
		switch (GetEvent().window.event)
		{
			case SDL_WINDOWEVENT_RESIZED: /* fall */
			case SDL_WINDOWEVENT_RESTORED: 
				if (m_resizeClbk) 
					m_resizeClbk(m_resizeClbkArg);  
				
				break;
			
			case SDL_WINDOWEVENT_CLOSE: 
				if (m_closeClbk) 
					m_closeClbk(m_closeClbkArg); 
				
				break;
		}

		return true;
	}

	
	return false;
}




void SdlRender::DrawBuffer()
{
	SDL_UpdateTexture(m_texture, nullptr, m_buffer, m_pitch);
	SDL_RenderCopy(m_rend, m_texture, nullptr, nullptr);
	SDL_RenderPresent(m_rend);
}





void SdlRender::SetWinCloseCallback(const void* arg, WinCloseCallback callback)
{
	m_closeClbkArg = arg;
	m_closeClbk = callback;
}




void SdlRender::SetWinResizeCallback(const void* arg, WinResizeCallback callback)
{
	m_resizeClbkArg = arg;
	m_resizeClbk = callback;
}










}
