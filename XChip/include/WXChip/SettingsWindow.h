/*

WXChip - wxWidgets GUI for XChip emulator.
Copyright (C) 2016  Jared Bruni.


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

#ifndef _WXCHIP_SETTINGS_WINDOW_H_
#define _WXCHIP_SETTINGS_WINDOW_H_

#include <wx/frame.h>

#include <XChip/Utility/Memory.h>

class SettingsWindow : public wxFrame
{
public:
	enum { ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3, ID_BTN_OK, ID_BTN_CANCEL, ID_BTN_DEFAULT };
	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
	const std::string GetRomPath() const;
	void SetRomPath(const std::string &text, const std::string &fps, const std::string &cpufreq);
	void CreateControls();
	void SaveSettings();
	const std::string RomPath() const;
	const std::string CPUFreq() const;
	const std::string FPS() const;

private:
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	void OnDefault(wxCommandEvent &event);


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
	wxDECLARE_EVENT_TABLE();
};



#endif
