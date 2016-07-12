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

#ifndef XCHIP_PLUGINS_SDLRENDER_H_
#define XCHIP_PLUGINS_SDLRENDER_H_
#include <SDL2/SDL.h>
#include <XChip/Plugins/iRender.h>

 
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
	bool Initialize(const utix::Vec2i& winSize, const utix::Vec2i& res) noexcept override;
	void Dispose() noexcept override;
	
	bool IsInitialized() const noexcept override;
	const char* GetPluginName() const noexcept override;
	const char* GetPluginVersion() const noexcept override;
	PluginDeleter GetPluginDeleter() const noexcept override;
	const char* GetWindowName() const noexcept override;
	const uint32_t* GetBuffer() const noexcept override;
	utix::Color GetDrawColor() const noexcept override;
	utix::Color GetBackgroundColor() const noexcept override;
	utix::Vec2i GetResolution() const noexcept override;
	utix::Vec2i GetWindowSize() const noexcept override;
	utix::Vec2i GetWindowPosition() const noexcept override;

	void SetWindowName(const char* name) noexcept override;
	void SetBuffer(const uint32_t* gfx) noexcept override;
	bool SetResolution(const utix::Vec2i& res) noexcept override;
	void SetWindowSize(const utix::Vec2i& size) noexcept override;
	void SetWindowPosition(const utix::Vec2i& pos) noexcept override;
	bool SetDrawColor(const utix::Color& color) noexcept override;
	bool SetBackgroundColor(const utix::Color& color) noexcept override;
	bool SetFullScreen(const bool option) noexcept override;
	bool UpdateEvents() noexcept override;
	void DrawBuffer() noexcept override;
	void HideWindow() noexcept override;
	void ShowWindow() noexcept override;

	void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept override;
	void SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept override;

private:
	bool CreateTexture(const int w, const int h);
	SDL_Event m_sdlevent;
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_rend = nullptr;
	SDL_Texture* m_texture = nullptr;
	const uint32_t* m_buffer = nullptr;
	WinCloseCallback m_closeClbk = nullptr;
	WinResizeCallback m_resizeClbk = nullptr;
	const void* m_closeClbkArg;
	const void* m_resizeClbkArg;
	int m_pitch;
	bool m_initialized = false;
};























}





#endif // XCHIP_PLUGINS_SDLRENDER_H_
