#ifndef _WXCHIP_SETTINGS_WINDOW_H_
#define _WXCHIP_SETTINGS_WINDOW_H_

#include <wx/frame.h>

#include <XChip/Utility/Memory.h>

class SettingsWindow : public wxFrame
{
public:
	enum { ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3, ID_BTN_OK, ID_BTN_CANCEL, ID_BTN_DEFAULT };
	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
	void setRomPath(const std::string &text, const std::string &fps, const std::string &cpufreq);
	void CreateControls();
	void SaveSettings();
	const std::string RomPath() const;
	const std::string CPUFreq() const;
	const std::string FPS() const;

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
	std::unique_ptr<wxButton> _buttonDefault;
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	void OnDefault(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
};



#endif
