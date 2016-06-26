/*

WXChip - chip8 emulator using XChip library and a wxWidgets gui.
Copyright (C) 2016  Jared Bruni, Rafael Moura.


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
#include <wx/string.h>
#include <wx/colourdata.h>

#include <Utix/Color.h>
#include <Utix/Vector2.h>
#include <Utix/Memory.h>

class SettingsWindow : public wxFrame
{
	using Color = utix::Color;
	using Vec2i = utix::Vec2i;
	static constexpr const auto* const default_cpu_hz = _T("380");
	static constexpr const auto* const default_fps = _T("60.00");
	static constexpr const char* const default_render_relative_path = "/bin/plugins/XChipSDLRender";
	static constexpr const char* const default_input_relative_path = "/bin/plugins/XChipSDLInput";
	static constexpr const char* const default_sound_relative_path = "/bin/plugins/XChipSDLSound";
	static constexpr const char* const default_bkg_color_str = "0x0x0";
	static constexpr const char* const default_fg_color_str = "255x255x255";
	static constexpr const unsigned long default_bkg_color = 0x0000000;
	static constexpr const unsigned long default_fg_color = 0x00ffffff;
	// these plugin default values need to evaluate at runtime
	// to be able to get the WXChip's working directory
	wxString default_render_full_path;
	wxString default_input_full_path;
	wxString default_sound_full_path;
public:
	enum 
	{ 
		ID_ROMS_DIR_TEXT = 1, ID_ROMS_DIR_TEXT_CTRL, 
		ID_FPS_TEXT, ID_FPS_TEXT_CTRL, 
		ID_CPU_HZ_TEXT, ID_CPU_HZ_TEXT_CTRL,
		ID_BUTTON_BKG_COLOR, ID_BUTTON_FG_COLOR, 
		ID_BUTTON_RENDER, ID_RENDER_TEXT_CTRL,
		ID_BUTTON_INPUT, ID_INPUT_TEXT_CTRL, 
		ID_BUTTON_SOUND, ID_SOUND_TEXT_CTRL,
		ID_BUTTON_OK, ID_BUTTON_CANCEL, ID_BUTTON_DEFAULT
	};

	SettingsWindow(const wxString &title, const wxPoint &pos);

	const std::string& GetArguments() const;
	std::string GetDirPath() const;	
	void SetDirPath(const std::string& path);
	void SaveSettings();
private:
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent& event);
	void OnOkay(wxCommandEvent& event);
	void OnDefault(wxCommandEvent& event);
	void OnSetRenderPlugin(wxCommandEvent& event);
	void OnSetInputPlugin(wxCommandEvent& event);
	void OnSetSoundPlugin(wxCommandEvent& event);
	void OnSetBKGColor(wxCommandEvent& event);
	void OnSetFGColor(wxCommandEvent& event);
	void UpdateConfigStr();
	void CreateControls();
	void ResetSettings();


	wxColour _bkgColor;
	wxColour _fgColor;

	std::string _configStr;
	std::string _bkgColorStr;
	std::string _fgColorStr;

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _romsDirTxt;
	std::unique_ptr<wxTextCtrl> _romsDirTxtCtrl;
	std::unique_ptr<wxStaticText> _fpsTxt;
	std::unique_ptr<wxTextCtrl> _fpsTxtCtrl;
	std::unique_ptr<wxStaticText> _cpuHzTxt;
	std::unique_ptr<wxTextCtrl> _cpuHzTxtCtrl;
	std::unique_ptr<wxButton> _buttonBKGColor;
	std::unique_ptr<wxButton> _buttonFGColor;
	std::unique_ptr<wxButton> _buttonRender;
	std::unique_ptr<wxButton> _buttonInput;
	std::unique_ptr<wxButton> _buttonSound;
	std::unique_ptr<wxTextCtrl> _renderTxtCtrl;
	std::unique_ptr<wxTextCtrl> _inputTxtCtrl;
	std::unique_ptr<wxTextCtrl> _soundTxtCtrl;
	std::unique_ptr<wxButton> _buttonOk;
	std::unique_ptr<wxButton> _buttonCancel;
	std::unique_ptr<wxButton> _buttonDefault;
	
	wxDECLARE_EVENT_TABLE();
};



inline const std::string& SettingsWindow::GetArguments() const { return _configStr;  }





#endif
