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
#include <XChip/Utility/Vector2.h>
#include <XChip/Utility/Memory.h>

class SettingsWindow : public wxFrame
{
	using Color = xchip::utility::Color;
	using Vec2i = xchip::utility::Vec2i;
	static constexpr int defaultCpuFreq = 380;
	static constexpr float defaultSoundFreq = 350.f;
	static constexpr float defaultFPS = 60.f;
	static constexpr Color defaultDrawColor = { 255, 255, 255 };
	static constexpr Color defaultBkgColor = { 0, 0, 0 };
public:
	enum 
	{ 
		ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, 
		ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3, 
		ID_BTN_OK, ID_BTN_CANCEL, ID_BTN_DEFAULT,
		ID_TEXT4, ID_RES
	};

	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

	std::string GetEmuConfigStr() const;
	const Color& GetDrawColor() const;
	const Color& GetBackgroundColor() const;
	Vec2i GetEmuResolution() const;
	std::string GetDirPath() const;
	int GetCPUFreq() const;
	float GetFPS() const;

	
	void SetDirPath(const std::string& path);
	void SetCPUFreq(const int freq);
	void SetSoundFreq(const float freq);
	void SetFPS(const float fps);
	void SetDrawColor(const Color& drawColor);
	void SetBackgroundColor(const Color& backgroundColor);
	void SetEmuWinSize(const Vec2i& size);
	void SetEmuFullscreen(const bool val);
	void SaveSettings();

private:
	void OnCloseWindow(wxCloseEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnOkay(wxCommandEvent &event);
	void OnDefault(wxCommandEvent &event);
	void CreateControls();
	void ResetTextControls();
	void ResetVariables();

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _dirTxt;
	std::unique_ptr<wxTextCtrl> _dirTxtCtrl;
	std::unique_ptr<wxStaticText> _fpsTxt;
	std::unique_ptr<wxTextCtrl> _fpsTxtCtrl;
	std::unique_ptr<wxStaticText> _cpuFreqTxt;
	std::unique_ptr<wxTextCtrl> _cpuFreqTxtCtrl;
	std::unique_ptr<wxStaticText> _emuResText;
	std::unique_ptr<wxComboBox> _emuResCBox;
	std::unique_ptr<wxButton> _buttonOk;
	std::unique_ptr<wxButton> _buttonCancel;
	std::unique_ptr<wxButton> _buttonDefault;

	Color _drawColor = defaultDrawColor;
	Color _bkgColor = defaultBkgColor;
	int _cpuFreq = defaultCpuFreq;
	float _soundFreq = defaultSoundFreq;
	float _fps = defaultFPS;


	wxDECLARE_EVENT_TABLE();
};










inline int SettingsWindow::GetCPUFreq() const { return _cpuFreq; }
inline float SettingsWindow::GetFPS() const { return _fps; }
inline const SettingsWindow::Color& SettingsWindow::GetDrawColor() const { return _drawColor; }
inline const SettingsWindow::Color& SettingsWindow::GetBackgroundColor() const { return _bkgColor; }



inline void SettingsWindow::SetDrawColor(const Color& drawColor) { _drawColor = drawColor; }
inline void SettingsWindow::SetBackgroundColor(const Color& backgroundColor) { _bkgColor = backgroundColor; }





#endif
