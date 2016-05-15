#ifndef _XCHIP_IRENDER_H_
#define _XCHIP_IRENDER_H_
#include <XChip/Utility/StdintDef.h>
#include <XChip/Utility/Color.h>
#include <XChip/Utility/Resolution.h>
#include "iMediaInterface.h"
 
namespace xchip {



class iRender : public iMediaInterface
{
public:
	enum class ScrollType { InPixels, InLines };
	using WinCloseCallback = void(*)(const void*);
	using WinResizeCallback = void(*)(const void*);

	virtual bool Initialize(const int w, const int h) noexcept = 0;
	
	virtual const char* GetWindowName() const noexcept = 0;
	virtual const uint32_t* GetBuffer() const noexcept = 0;
	virtual utility::Color GetColorFilter() const noexcept = 0;
	virtual utility::Resolution GetResolution() const noexcept = 0;
	virtual int GetScrollX(const ScrollType type = ScrollType::InPixels) const noexcept = 0;
	virtual int GetScrollY(const ScrollType type = ScrollType::InLines) const noexcept = 0;


	virtual bool UpdateEvents() noexcept = 0;
	virtual void SetWindowName(const char* name) noexcept = 0;
	virtual bool SetColorFilter(const utility::Color& color) noexcept = 0;
	virtual bool SetResolution(const utility::Resolution& res) noexcept = 0;
	virtual bool SetFullScreen(const bool value) noexcept = 0;
	virtual void SetScrollX(const int x, const ScrollType type = ScrollType::InPixels) noexcept = 0;
	virtual void SetScrollY(const int y, const ScrollType type = ScrollType::InLines) noexcept = 0;
	virtual void SetBuffer(const uint32_t* gfx) noexcept = 0;
	virtual void DrawBuffer() noexcept = 0;
	virtual void HideWindow() noexcept = 0;
	virtual void ShowWindow() noexcept = 0;
	virtual void SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept = 0;
	virtual void SetWinResizeCallback(const void* arg, WinResizeCallback callack) noexcept = 0;
};















}

#endif
