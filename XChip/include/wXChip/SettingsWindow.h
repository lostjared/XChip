#ifndef __SETTINGS__H__
#define __SETTINGS__H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <XChip/Utility/Memory.h>

class SettingsWindow: public wxFrame
{
public:
	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
	void setRomPath(const std::string &text);
	void CreateControls();
	void SaveSettings();
private:
	float _fpsValue;
	unsigned int _freqValue;
	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _romText;
	std::unique_ptr<wxTextCtrl> _romPath;
	std::unique_ptr<wxStaticText> _fpsText;
	std::unique_ptr<wxTextCtrl> _fps;
	std::unique_ptr<wxStaticText> _cpuText;
	std::unique_ptr<wxTextCtrl> _cpu;
	std::unique_ptr<wxButton> _buttonOk;
	std::unique_ptr<wxButton> _buttonCancel;
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
};



#endif
