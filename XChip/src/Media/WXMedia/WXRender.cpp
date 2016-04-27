#include <XChip/Media/WXMedia/WXRender.h>


#include <XChip/Media/SDLMedia/WXRender.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>

#define _INITIALIZED_ASSERT_ ASSERT_MSG(_initialized == true, "WXRender is not initialized");

namespace xchip {
	
	
	WXRender::WXRender() noexcept
	: SdlSystem(System::Render)
	
	{
		utility::LOG("Creating WXRenderer object...");
	}
	
	
	WXRender::~WXRender()
	{
		utility::LOG("Destroying WXRenderer object...");
		if (_initialized)
			this->Dispose();
	}
	
	
	bool WXRender::Initialize(const int width, const int height) noexcept
	{
		if (_initialized)
			this->Dispose();
			
		
		_initialized = true;
		
		return true;
	}
	
	
	void WXRender::Dispose() noexcept
	{

	}
	
	
	
	
	
	bool WXRender::UpdateEvents() noexcept
	{
		_INITIALIZED_ASSERT_;
		return false;
	}
	
	
	bool WXRender::SetColorFilter(const utility::Color& color) noexcept
	{
		_INITIALIZED_ASSERT_;
		return true;
	}
	
	void WXRender::DrawBuffer() noexcept
	{
	
	}
	
	
	void WXRender::HideWindow() noexcept
	{
	}
	
	void WXRender::ShowWindow() noexcept
	{

	}
	
	
	
	void WXRender::SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept
	{
		_closeClbkArg = arg;
		_closeClbk = callback;
	}
	
	
	
	
	void WXRender::SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept
	{
		_resizeClbkArg = arg;
		_resizeClbk = callback;
	}
	
	
	
	
	utility::Color WXRender::GetColorFilter() const noexcept
	{
		_INITIALIZED_ASSERT_;
		utility::Color color;
		return color;
		
	}
	
}
