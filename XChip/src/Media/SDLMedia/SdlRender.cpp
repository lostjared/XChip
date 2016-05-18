#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>

#define _SDLRENDER_INITIALIZED_ASSERT_() ASSERT_MSG(_initialized == true, "SdlRender is not initialized")

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





bool SdlRender::Initialize(const utility::Vec2i& winSize, const utility::Vec2i& res) noexcept
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

	_pitch = res.x * sizeof(uint32_t);

	_window = SDL_CreateWindow("Chip8 - SdlRender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                   winSize.x, winSize.y, SDL_WINDOW_RESIZABLE);

	if (!_window) 
		return false;

	_rend = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (!_rend)
		return false;

	_texture = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, res.x, res.y);

	if (!_texture)
		return false;

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





const char* SdlRender::GetWindowName() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	return SDL_GetWindowTitle(_window);
}


utility::Color SdlRender::GetColorFilter() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	utility::Color color;
	SDL_GetTextureColorMod(_texture, &color.r, &color.g, &color.b);
	return color;

}


utility::Color SdlRender::GetBackgroundColor() const noexcept
{


	return {0, 0, 0};
}



utility::Vec2i SdlRender::GetResolution() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	
	utility::Vec2i res;
	if( SDL_QueryTexture(_texture, nullptr, nullptr, &res.x, &res.y) )
	{
		xchip::utility::LOGerr(SDL_GetError());
		return {0, 0};
	}

	return res;
}


utility::Vec2i SdlRender::GetWindowSize() const noexcept
{
	utility::Vec2i size;
	SDL_GetWindowSize(_window, &size.x, &size.y);
	return size;
}



bool SdlRender::UpdateEvents() noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
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



void SdlRender::SetWindowName(const char* name) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_SetWindowTitle(_window, name);
}


bool SdlRender::SetResolution(const utility::Vec2i& res) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	using namespace utility::literals;

	_pitch = res.x * sizeof(uint32_t);
	
	const auto currentColor = this->GetColorFilter();
	SDL_Texture* const oldTexture = _texture;

	_texture = SDL_CreateTexture(_rend, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, res.x, res.y);

	if (!_texture)
	{
		utility::LOGerr("Could not create new texture: "_s + SDL_GetError());
		_texture = oldTexture;
		return false;
	}

	SDL_DestroyTexture(oldTexture);
	this->SetColorFilter(currentColor);

	return true;
}



void SdlRender::SetWindowSize(const utility::Vec2i& size) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_SetWindowSize(_window, size.x, size.y);
}



bool SdlRender::SetFullScreen(const bool val) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();

	using namespace utility::literals;


	if(val)
	{
		if(SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			utility::LOGerr("Error while setting SdlRender Fullscreen: "_s + SDL_GetError());
			return false;
		}

		if(SDL_ShowCursor(0) < 0)
		{
			utility::LOGerr("Error while hiding cursor: "_s + SDL_GetError());
		}
	}

	else
	{

		if(SDL_SetWindowFullscreen(_window, 0))
		{
			utility::LOGerr("Error while setting SdlRender Windowed: "_s + SDL_GetError());
			return false;
		}
	}

	return true;
}








bool SdlRender::SetColorFilter(const utility::Color& color) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	if(SDL_SetTextureColorMod(_texture, color.r, color.g, color.b))
	{
		xchip::utility::LOGerr(SDL_GetError());
		return false;
	}
	return true;
}



bool SdlRender::SetBackgroundColor(const utility::Color& color) noexcept
{
	//SDL_SetSurfaceColorMod(SDL_GetWindowSurface(_window), color.r, color.g, color.b);
	//SDL_UpdateWindowSurface(_window);
	return true;
}












void SdlRender::DrawBuffer() noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	ASSERT_MSG(_buffer != nullptr, "attempt to draw null buffer");
	SDL_RenderClear(_rend);
	SDL_UpdateTexture(_texture, nullptr, _buffer, _pitch);
	SDL_RenderCopy(_rend, _texture, nullptr, nullptr);
	SDL_RenderPresent(_rend);
}


void SdlRender::HideWindow() noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_HideWindow(_window);
}

void SdlRender::ShowWindow() noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_ShowWindow(_window);
}



void SdlRender::SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept
{
	_closeClbkArg = arg;
	_closeClbk = callback;
}




void SdlRender::SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept 
{
	_resizeClbkArg = arg;
	_resizeClbk = callback;
}









}
