#ifndef __SETTINGS__H__
#define __SETTINGS__H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <XChip/Utility/Memory.h>

class SettingsWindow: public wxFrame {
public:
	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
	void setRomPath(const std::string &text);
	void CreateControls() noexcept;
private:
	
	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _romText;
	std::unique_ptr<wxTextCtrl> _romPath;
	
	void OnCloseWindow(wxCloseEvent &event);
	
	wxDECLARE_EVENT_TABLE();
};



#endif
