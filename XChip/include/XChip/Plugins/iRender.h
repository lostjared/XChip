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

#ifndef XCHIP_PLUGINS_IRENDER_H_
#define XCHIP_PLUGINS_IRENDER_H_
#include <Utix/Ints.h>
#include <Utix/Color.h>
#include <Utix/Vector2.h>
#include "iPlugin.h"
 
namespace xchip {



class iRender : public iPlugin
{
public:
	using WinCloseCallback = void(*)(const void*);
	using WinResizeCallback = void(*)(const void*);

	virtual bool Initialize(const utix::Vec2i& winSize, const utix::Vec2i& resolution) noexcept = 0;
	
	virtual const char* GetWindowName() const noexcept = 0;
	virtual const uint32_t* GetBuffer() const noexcept = 0;
	virtual utix::Vec2i GetResolution() const noexcept = 0;
	virtual utix::Vec2i GetWindowSize() const noexcept = 0;
	virtual utix::Vec2i GetWindowPosition() const noexcept = 0;
	virtual utix::Color GetDrawColor() const noexcept = 0;
	virtual utix::Color GetBackgroundColor() const noexcept = 0;

	virtual bool UpdateEvents() noexcept = 0;
	virtual void SetWindowName(const char* name) noexcept = 0;
	virtual bool SetResolution(const utix::Vec2i& res) noexcept = 0;
	virtual void SetWindowSize(const utix::Vec2i& size) noexcept = 0;
	virtual void SetWindowPosition(const utix::Vec2i& pos) noexcept = 0;
	virtual bool SetFullScreen(const bool value) noexcept = 0;
	virtual bool SetDrawColor(const utix::Color& color) noexcept = 0;
	virtual bool SetBackgroundColor(const utix::Color& color) noexcept = 0;
	virtual void SetBuffer(const uint32_t* gfx) noexcept = 0;
	virtual void DrawBuffer() noexcept = 0;
	virtual void HideWindow() noexcept = 0;
	virtual void ShowWindow() noexcept = 0;
	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept = 0;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callack) noexcept = 0;
};















}

#endif // XCHIP_PLUGINS_IRENDER_H_
