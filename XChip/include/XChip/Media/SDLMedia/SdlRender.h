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

#ifndef _XCHIP_SDLRENDER_H_
#define _XCHIP_SDLRENDER_H_
#include <SDL2/SDL.h>
#include <XChip/Media/iRender.h>

 
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

namespace xchip {


class SdlRender final : public iRender
{
	static constexpr const char* const PLUGIN_NAME = "SdlRender";
	static constexpr const char* const PLUGIN_VER = "SdlRender 1.0. Using SDL2";
public:
	SdlRender() noexcept;
	~SdlRender();
	bool Initialize(const utility::Vec2i& winSize, const utility::Vec2i& res) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	const char* GetPluginName() const noexcept override;
	const char* GetPluginVersion() const noexcept override;
	MediaPluginDeleter GetPluginDeleter() const noexcept override;
	const char* GetWindowName() const noexcept override;
	const uint32_t* GetBuffer() const noexcept override;
	utility::Color GetDrawColor() const noexcept override;
	utility::Color GetBackgroundColor() const noexcept override;
	utility::Vec2i GetResolution() const noexcept override;
	utility::Vec2i GetWindowSize() const noexcept override;
	
	void SetWindowName(const char* name) noexcept override;
	void SetBuffer(const uint32_t* gfx) noexcept override;
	bool SetDrawColor(const utility::Color& color) noexcept override;
	bool SetBackgroundColor(const utility::Color& color) noexcept override;
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
	bool CreateTexture(const int w, const int h);
	SDL_Event _sdlevent;
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























}
#endif
