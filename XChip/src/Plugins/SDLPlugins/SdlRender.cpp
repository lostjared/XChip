/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#include <cstdlib>

#include <XChip/Plugins/SDLPlugins/SdlRender.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>

#define _SDLRENDER_INITIALIZED_ASSERT_() ASSERT_MSG(_initialized == true, "SdlRender is not initialized")

namespace xchip {

using namespace utility;

extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin*);









SdlRender::SdlRender() noexcept
{
	LOG("Creating SdlRenderer object...");
}


SdlRender::~SdlRender()
{
	LOG("Destroying SdlRenderer object...");
	if (_initialized)
		this->Dispose();
}


bool SdlRender::Initialize(const Vec2i& winSize, const Vec2i& res) noexcept
{
	using namespace literals;

	if (_initialized)
		this->Dispose();


	if( SDL_InitSubSystem( SDL_INIT_VIDEO ) )
	{
		LOGerr("Could not initialize SDL2 Video: "_s + SDL_GetError());
		return false;
	}
	
	const auto scope = make_scope_exit([this]() noexcept 
	{
		if (!this->_initialized) 
		{
			LOGerr("Couldn't initialize SdlRender. SDL ERROR MSG: "_s + SDL_GetError());
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


	if(!CreateTexture(res.x, res.y))
		return false;


	SDL_SetRenderDrawColor(_rend, 0, 0, 0, 0xff);
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
	SDL_QuitSubSystem( SDL_INIT_VIDEO );
	_window = nullptr;
	_buffer = nullptr;
	_closeClbk = nullptr;
	_resizeClbk = nullptr;
	_initialized = false;

}



bool SdlRender::IsInitialized() const noexcept 
{ 
	return _initialized; 
}


const char* SdlRender::GetPluginName() const noexcept
{
	return PLUGIN_NAME;
}

const char* SdlRender::GetPluginVersion() const noexcept
{
	return PLUGIN_VER;
}

PluginDeleter SdlRender::GetPluginDeleter() const noexcept
{
	return XCHIP_FreePlugin;
}



const uint32_t* SdlRender::GetBuffer() const noexcept 
{ 
	return _buffer; 
}

const char* SdlRender::GetWindowName() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	return SDL_GetWindowTitle(_window);
}


Color SdlRender::GetDrawColor() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();

	Color color;
	SDL_GetTextureColorMod(_texture, &color.r, &color.g, &color.b);
	return color;

}


Color SdlRender::GetBackgroundColor() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	using namespace literals;

	Color color;
	
	if(SDL_GetRenderDrawColor(_rend, &color.r, &color.g, &color.b, nullptr))
	{
		LOGerr("Could not get render draw color: "_s + SDL_GetError());
		return {0, 0, 0};
	}

	return color;
}





Vec2i SdlRender::GetResolution() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	
	using namespace literals;


	Vec2i res;
	if( SDL_QueryTexture(_texture, nullptr, nullptr, &res.x, &res.y) )
	{
		LOGerr("Failed to get SDL_Texture resolution: "_s + SDL_GetError());
		return {0, 0};
	}

	return res;
}




Vec2i SdlRender::GetWindowSize() const noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();

	Vec2i size;
	SDL_GetWindowSize(_window, &size.x, &size.y);
	return size;
}





bool SdlRender::UpdateEvents() noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();


	bool hasEvent = false;
	while (SDL_PollEvent(&_sdlevent) != 0)
	{
		if (_sdlevent.type == SDL_WINDOWEVENT)
		{
			switch (_sdlevent.window.event)
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

			hasEvent = true;		
		}

	}

	return hasEvent;
}






void SdlRender::SetBuffer(const uint32_t* gfx) noexcept 
{ 
	_buffer = gfx;
}





void SdlRender::SetWindowName(const char* name) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_SetWindowTitle(_window, name);
}




bool SdlRender::SetResolution(const Vec2i& res) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	using namespace literals;

	_pitch = res.x * sizeof(uint32_t);
	
	const auto currentColor = this->GetDrawColor();

	if(!CreateTexture(res.x, res.y))
		return false;

	this->SetDrawColor(currentColor);

	return true;
}





void SdlRender::SetWindowSize(const Vec2i& size) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	SDL_SetWindowSize(_window, size.x, size.y);
}






bool SdlRender::SetFullScreen(const bool val) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();

	using namespace literals;


	if(val)
	{
		if(SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			LOGerr("Error while setting SdlRender Fullscreen: "_s + SDL_GetError());
			return false;
		}

		else if(SDL_ShowCursor(0) < 0) 
		{
			LOGerr("Error while hiding cursor: "_s + SDL_GetError());
		}
	}

	else
	{

		if(SDL_SetWindowFullscreen(_window, 0))
		{
			LOGerr("Error while setting SdlRender Windowed: "_s + SDL_GetError());
			return false;
		}
	}


	return true;
}








bool SdlRender::SetDrawColor(const Color& color) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();
	
	using namespace literals;

	if(SDL_SetTextureColorMod(_texture, color.r, color.g, color.b))
	{
		LOGerr("Failed to set texture draw color: "_s + SDL_GetError());
		return false;
	}

	return true;
}






bool SdlRender::SetBackgroundColor(const Color& color) noexcept
{
	_SDLRENDER_INITIALIZED_ASSERT_();

	using namespace literals;

	if(SDL_SetRenderDrawColor(_rend, color.r, color.g, color.b, 0xff))
	{
		LOGerr("Could not set render draw color: "_s + SDL_GetError());
		return false;
	}

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




bool SdlRender::CreateTexture(const int w, const int h)
{
	using namespace literals;
	auto* const surface = SDL_CreateRGBSurface(0, w, h, 32, 0,0,0,0);

	if(!surface)
	{
		LOGerr("Could not create surface: "_s + SDL_GetError());
		return false;
	}

	auto* oldTexture = _texture;
	
	const auto cleanup = make_scope_exit([&surface, &oldTexture]() noexcept
	{
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(oldTexture);
	});


	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
	_texture = SDL_CreateTextureFromSurface(_rend, surface);

	if(!_texture)
	{
		LOGerr("Could not create texture: "_s + SDL_GetError());
		_texture = oldTexture;
		oldTexture = nullptr;
		return false;
	}

	return true;	
}












extern "C" XCHIP_EXPORT iPlugin* XCHIP_LoadPlugin()
{
	return new(std::nothrow) SdlRender();
}


extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin)
{
	const auto* sdlrend = dynamic_cast<const SdlRender*>(plugin);

	if(!sdlrend)
	{
		LOGerr("XCHIP_FreePlugin: dynamic_cast from iPlugin* to SdlRender* Failed");
		std::exit(EXIT_FAILURE);
	}

	delete sdlrend;
}











}
