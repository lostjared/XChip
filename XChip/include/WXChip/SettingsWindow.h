/*

WXChip - chip8 emulator using XChip library and a wxWidgets gui.
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
#include <XChip/Utility/Color.h>
#include <XChip/Utility/Vector2.h>
#include <XChip/Utility/Memory.h>

class SettingsWindow : public wxFrame
{
	using Color = xchip::utility::Color;
	using Vec2i = xchip::utility::Vec2i;
	static constexpr const auto* const defaultCpuFreq = _T("380");
	static constexpr const auto* const defaultFPS = _T("60.00");

public:
	enum 
	{ 
		ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, 
		ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3, 
		ID_BTN_OK, ID_BTN_CANCEL, ID_BTN_DEFAULT,
		ID_TEXT4, ID_RES, ID_PLUGRENDIR, ID_PLUGRENTEXT,
		ID_PLUGINDIR, ID_PLUGINTEXT, ID_PLUGSNDDIR, ID_PLUGSNDTEXT
	};

	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

	const std::string& GetEmuConfigStr() const;
	std::string GetDirPath() const;
	int GetCPUFreq() const;
	float GetFPS() const;

	
	void SetDirPath(const std::string& path);
	void SetCPUFreq(const int freq);
	void SetFPS(const float fps);
	void SaveSettings();

private:
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	void OnDefault(wxCommandEvent &event);
	void OnSetRenderPlugin(wxCommandEvent &event);
	void OnSetInputPlugin(wxCommandEvent &event);
	void OnSetSoundPlugin(wxCommandEvent &event);
	void UpdateConfigStr();
	void CreateControls();
	void ResetTextControls();

	std::string _configStr;
	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _dirTxt;
	std::unique_ptr<wxTextCtrl> _dirPath;
	std::unique_ptr<wxStaticText> _fpsTxt;
	std::unique_ptr<wxTextCtrl> _fps;
	std::unique_ptr<wxStaticText> _cpuHzTxt;
	std::unique_ptr<wxTextCtrl> _cpuHz;
	std::unique_ptr<wxStaticText> _emuResText;
	std::unique_ptr<wxComboBox> _emuRes;
	std::unique_ptr<wxCheckBox> _emuFullScreen;
	std::unique_ptr<wxButton> _buttonOk;
	std::unique_ptr<wxButton> _buttonCancel;
	std::unique_ptr<wxButton> _buttonDefault;
	std::unique_ptr<wxButton> _plugRenDir;
	std::unique_ptr<wxTextCtrl> _plugRenText;
	std::unique_ptr<wxButton> _plugInDir;
	std::unique_ptr<wxTextCtrl> _plugInText;
	std::unique_ptr<wxButton> _plugSndDir;
	std::unique_ptr<wxTextCtrl> _plugSndText;
	
	wxDECLARE_EVENT_TABLE();
};



inline const std::string& SettingsWindow::GetEmuConfigStr() const { return _configStr;  }





#endif
