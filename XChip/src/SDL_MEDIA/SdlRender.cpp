#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include <XChip/SDL_MEDIA/SdlRender.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>



namespace xchip {

extern SDL_Event g_sdlEvent;

SdlRender::SdlRender() noexcept
	: SdlSystem(System::Render)

{
	utility::LOG("Creating SdlRenderer object...");
}


SdlRender::~SdlRender()
{
	utility::LOG("Destroying SdlRenderer object...");
	if (_initialized)
		this->Dispose();
}





bool SdlRender::Initialize(const int width, const int height) noexcept
{
	if (_initialized)
		this->Dispose();

	else if (!this->InitSubSystem())
		return false;

	const auto scope = utility::make_scope_exit([this]() noexcept 
	{
		using namespace xchip::utility::literals;
		if (!this->_initialized) {
			utility::LOGerr("Couldn't initialize SdlRender. SDL ERROR MSG: "_s + SDL_GetError());
			this->Dispose();
		}
	});

	_pitch = width * 4;
	_window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                   width * 4, height * 6, SDL_WINDOW_RESIZABLE);

	if (!_window) 
		return false;

	_rend = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (!_rend)
		return false;

	_texture = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (!_texture)
		return false;

	SDL_SetRenderDrawColor(_rend, 55, 55, 55, 255);
	SDL_RenderClear(_rend);
	SDL_RenderPresent(_rend);

	_initialized = true;
	return true;
}


void SdlRender::Dispose() noexcept
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyRenderer(_rend);
	SDL_DestroyWindow(_window);
	_window = nullptr;
	_buffer = nullptr;
	_closeClbk = nullptr;
	_resizeClbk = nullptr;
	_initialized = false;
}





bool SdlRender::UpdateEvents()
{
	PollEvent();
	
	if (g_sdlEvent.type == SDL_WINDOWEVENT)
	{
		switch (g_sdlEvent.window.event)
		{
			case SDL_WINDOWEVENT_RESIZED: /* fall */
			case SDL_WINDOWEVENT_RESTORED: 
				if (_resizeClbk) 
					_resizeClbk(_resizeClbkArg);  
				
				break;
			
			case SDL_WINDOWEVENT_CLOSE: 
				if (_closeClbk) 
					_closeClbk(_closeClbkArg); 
				
				break;
		}

		return true;
	}

	
	return false;
}




void SdlRender::DrawBuffer()
{
	ASSERT_MSG(_buffer != nullptr,
		"SdlRender::DrawBuffer: attempt to draw null buffer");

	SDL_UpdateTexture(_texture, nullptr, _buffer, _pitch);
	SDL_RenderCopy(_rend, _texture, nullptr, nullptr);
	SDL_RenderPresent(_rend);
}





void SdlRender::SetWinCloseCallback(const void* arg, WinCloseCallback callback)
{
	_closeClbkArg = arg;
	_closeClbk = callback;
}




void SdlRender::SetWinResizeCallback(const void* arg, WinResizeCallback callback)
{
	_resizeClbkArg = arg;
	_resizeClbk = callback;
}










}
