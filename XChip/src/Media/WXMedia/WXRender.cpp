#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <XChip/Media/WXMedia/WXRender.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>

#define _INITIALIZED_ASSERT_ ASSERT_MSG(_initialized == true, "WXRender is not initialized");

namespace xchip {
	
	
	class WXRenderFrame : public wxFrame {
	public:
		WXRenderFrame() :  wxFrame(NULL, wxID_ANY, "XChip", wxPoint(640,480), wxSize(320, 240), wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
		{
			
		}
		
	};
	
	
	WXRenderFrame *render_frame = 0;
	
	
	WXRender::WXRender() noexcept
	
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
			
		
			render_frame = new WXRenderFrame();
			
			
		_initialized = true;
		
		return true;
	}
	
	
	void WXRender::Dispose() noexcept
	{
		if(render_frame)
		{
			delete render_frame;
			render_frame = nullptr;
		}
		
	}
	
	
	
	
	
	bool WXRender::UpdateEvents() noexcept
	{
		_INITIALIZED_ASSERT_;
		
		if(render_frame) render_frame->Update();
		
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
		if(render_frame) render_frame->Show(false);
	}
	
	void WXRender::ShowWindow() noexcept
	{
		if(render_frame) render_frame->Show(true);
	}
	
	
	
	void WXRender::SetWinCloseCallback(const void* arg, WinCloseCallback callback) noexcept
	{
		
	}
	
	
	void WXRender::SetWinResizeCallback(const void* arg, WinResizeCallback callback) noexcept
	{
	
	}
	
	
	
	
	utility::Color WXRender::GetColorFilter() const noexcept
	{
		_INITIALIZED_ASSERT_;
		utility::Color color;
		return color;
		
	}
	
}
