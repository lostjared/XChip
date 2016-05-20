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
#include <XChip/Utility/Color.h>
#include <XChip/Utility/Memory.h>

class SettingsWindow : public wxFrame
{
	using Color = xchip::utility::Color;
public:
	enum 
	{ 
		ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, 
		ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3, 
		ID_BTN_OK, ID_BTN_CANCEL, ID_BTN_DEFAULT 
	};

	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
	std::string GetDirPath() const;
	unsigned int GetCPUFreq() const;
	float GetFPS() const;
	
	void SetDirPath(const std::string& path);
	void SetCPUFreq(const unsigned int freq);
	void SetSoundFreq(const float freq);
	void SetFPS(const float fps);
	void SetDrawColor(const Color& drawColor);
	void SetBackgroundColor(const Color& backgroundColor);
	void SaveSettings();


private:
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	void OnDefault(wxCommandEvent &event);
	void CreateControls();

	unsigned int _cpuFreq;
	float _soundFreq;
	float _fpsValue;
	Color _drawColor;
	Color _backgroundColor;
	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _dirText;
	std::unique_ptr<wxTextCtrl> _dirPath;
	std::unique_ptr<wxStaticText> _fpsText;
	std::unique_ptr<wxTextCtrl> _fps;
	std::unique_ptr<wxStaticText> _cpuText;
	std::unique_ptr<wxTextCtrl> _cpu;
	std::unique_ptr<wxButton> _buttonOk;
	std::unique_ptr<wxButton> _buttonCancel;
	std::unique_ptr<wxButton> _buttonDefault;
	wxDECLARE_EVENT_TABLE();
};



inline unsigned int SettingsWindow::GetCPUFreq() const { return _cpuFreq; }
inline float SettingsWindow::GetFPS() const { return _fpsValue; }
	


inline void SettingsWindow::SetCPUFreq(const unsigned int freq) { _cpuFreq = freq; }
inline void SettingsWindow::SetSoundFreq(const float freq) { _soundFreq = freq; }
inline void SettingsWindow::SetFPS(const float fps) { _fpsValue = fps; }
inline void SettingsWindow::SetDrawColor(const Color& drawColor) { _drawColor = drawColor; }
inline void SettingsWindow::SetBackgroundColor(const Color& backgroundColor) { _backgroundColor = backgroundColor; }

#endif
