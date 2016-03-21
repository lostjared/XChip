#ifndef _XCHIP_IRENDER_H_
#define _XCHIP_IRENDER_H_
#include <cstdint>

namespace xchip {


class iRender
{
public:
	using WinCloseCallback = void(*)(const void*);
	using WinResizeCallback = void(*)(const void*);

	virtual ~iRender() = default;
	virtual bool Initialize(const int w, const int h) noexcept = 0;
	virtual void Dispose() noexcept = 0;
	virtual bool UpdateEvents() = 0;
	virtual void SetBuffer(const uint32_t* gfx) = 0;
	virtual void DrawBuffer() = 0;

	virtual void SetWinCloseCallback(WinCloseCallback callback, const void* arg) = 0;
	virtual void SetWinResizeCallback(WinResizeCallback callack, const void* arg) = 0;
};















}

#endif
