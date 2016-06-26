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

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <cstdlib>
#include <string>
#include <wx/valnum.h>
#include <wx/colordlg.h>
#include <Utix/Log.h>
#include <Utix/Common.h>
#include <WXChip/SaveList.h>
#include <WXChip/SettingsWindow.h>


static void ExecuteWxColourDialog(wxFrame* frame, wxColour* color, std::string* str);




wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
EVT_BUTTON(ID_BUTTON_OK, SettingsWindow::OnOkay)
EVT_BUTTON(ID_BUTTON_CANCEL, SettingsWindow::OnCancel)
EVT_BUTTON(ID_BUTTON_DEFAULT, SettingsWindow::OnDefault)
EVT_BUTTON(ID_BUTTON_RENDER, SettingsWindow::OnSetRenderPlugin)
EVT_BUTTON(ID_BUTTON_INPUT, SettingsWindow::OnSetInputPlugin)
EVT_BUTTON(ID_BUTTON_SOUND, SettingsWindow::OnSetSoundPlugin)
EVT_BUTTON(ID_BUTTON_BKG_COLOR, SettingsWindow::OnSetBKGColor)
EVT_BUTTON(ID_BUTTON_FG_COLOR, SettingsWindow::OnSetFGColor)
wxEND_EVENT_TABLE()


constexpr decltype(SettingsWindow::default_cpu_hz) SettingsWindow::default_cpu_hz;
constexpr decltype(SettingsWindow::default_fps) SettingsWindow::default_fps;

constexpr const unsigned long SettingsWindow::default_bkg_color;
constexpr const unsigned long SettingsWindow::default_fg_color;
constexpr const char* const SettingsWindow::default_bkg_color_str;
constexpr const char* const SettingsWindow::default_fg_color_str;
constexpr const char* const SettingsWindow::default_render_relative_path;
constexpr const char* const SettingsWindow::default_input_relative_path;
constexpr const char* const SettingsWindow::default_sound_relative_path;


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos)
	: wxFrame(NULL, wxID_ANY, title, pos, wxSize(450, 360), 
                  wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX),
	_bkgColor(default_bkg_color),
	_fgColor(default_fg_color)

{
	const wxString procPath = utix::GetFullProcDir();
	default_render_full_path = (procPath + default_render_relative_path);
	default_input_full_path = (procPath + default_input_relative_path);
	default_sound_full_path = (procPath + default_sound_relative_path);
	_bkgColorStr = default_bkg_color_str;
	_fgColorStr = default_fg_color_str;

	CreateControls();
	UpdateConfigStr();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}



std::string SettingsWindow::GetDirPath() const
{
	return static_cast<const char*>(_romsDirTxtCtrl->GetLineText(0).c_str());
}


void SettingsWindow::SetDirPath(const std::string &dirPath)
{
	_romsDirTxtCtrl->Clear();
	*_romsDirTxtCtrl << dirPath.c_str();
}


void SettingsWindow::UpdateConfigStr()
{
	_configStr = "";
	const auto AddArg = [this](const char* opt, const char* arg) {
		((((_configStr += ' ') += opt) += ' ') += arg)  += ' ';
	};


	AddArg("-FPS", _fpsTxtCtrl->GetLineText(0).c_str());
	AddArg("-CHZ", _cpuHzTxtCtrl->GetLineText(0).c_str());	
	AddArg("-REN",_renderTxtCtrl->GetLineText(0).c_str());
	AddArg("-INP", _inputTxtCtrl->GetLineText(0).c_str());
	AddArg("-SND",_soundTxtCtrl->GetLineText(0).c_str());
	AddArg("-COL", _fgColorStr.c_str());
	AddArg("-BKG", _bkgColorStr.c_str());
}


void SettingsWindow::CreateControls()
{
	using utix::make_unique;


	_panel = make_unique<wxPanel>(this, wxID_ANY);

	_romsDirTxt = make_unique<wxStaticText>(_panel.get(), ID_ROMS_DIR_TEXT,_T("Directory: "), 
                                            wxPoint(10,15), wxSize(80,25));

	_romsDirTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_ROMS_DIR_TEXT_CTRL, _T(""), 
                                            wxPoint(100,10), wxSize(320,20), wxTE_READONLY);

	// validators 
	wxIntegerValidator<int> cpuFreqValidator(nullptr, wxNUM_VAL_ZERO_AS_BLANK);
	cpuFreqValidator.SetRange(1, 5000);

	wxFloatingPointValidator<float> fpsValidator(2, nullptr, wxNUM_VAL_ZERO_AS_BLANK);
	fpsValidator.SetRange(1.00, 120.00);


	_cpuHzTxt = make_unique<wxStaticText>(_panel.get(), ID_CPU_HZ_TEXT,_T("CPU Hz: "), 
                                                 wxPoint(220,45), wxSize(60,25));

	_cpuHzTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_CPU_HZ_TEXT_CTRL, default_cpu_hz, 
                                                  wxPoint(320,40), wxSize(100,20), 0, cpuFreqValidator);

	_fpsTxt = make_unique<wxStaticText>(_panel.get(), ID_FPS_TEXT,_T("FPS: "), 
                                             wxPoint(10,45), wxSize(60,25));

	_fpsTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_FPS_TEXT_CTRL, default_fps,
                                               wxPoint(100,40), wxSize(100,20), 0, fpsValidator);


	_buttonBKGColor= make_unique<wxButton>(_panel.get(), ID_BUTTON_BKG_COLOR, _T("Background Color"),
                                                wxPoint(10, 75), wxSize(145, 35));

	_buttonFGColor= make_unique<wxButton>(_panel.get(), ID_BUTTON_FG_COLOR, _T("Frontground Color"),
                                            wxPoint(160, 75), wxSize(145, 35));

	_buttonRender = make_unique<wxButton>(_panel.get(), ID_BUTTON_RENDER, _T("Render Plugin"),
                                             wxPoint(10, 115), wxSize(140, 33));
	
	_buttonInput = make_unique<wxButton>(_panel.get(), ID_BUTTON_INPUT, _T("Input Plugin"),
                                            wxPoint(10, 155), wxSize(140, 33));

	_buttonSound = make_unique<wxButton>(_panel.get(), ID_BUTTON_SOUND, _T("Sound Plugin"),
                                         wxPoint(10, 195), wxSize(140, 33));
	
	_renderTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_RENDER_TEXT_CTRL, default_render_full_path,
                                                wxPoint(165, 120), wxSize(200, 20), wxTE_READONLY);

	_inputTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_INPUT_TEXT_CTRL, default_input_full_path,
                                           wxPoint(165, 160), wxSize(200, 20), wxTE_READONLY);

	_soundTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_SOUND_TEXT_CTRL, default_sound_full_path, 
                                           wxPoint(165, 200), wxSize(200, 20), wxTE_READONLY);

	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BUTTON_OK, _T("Ok"), 
                                           wxPoint(10, 250), wxSize(100, 35));

	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BUTTON_CANCEL, _T("Cancel"), 
                                               wxPoint(120, 250), wxSize(100, 35));

	_buttonDefault = make_unique<wxButton>(_panel.get(), ID_BUTTON_DEFAULT, _T("Default"), 
                                                wxPoint(230,250), wxSize(100, 35));
	
}



void SettingsWindow::SaveSettings()
{
//	saveDirectory((const char*)_dirTxtCtrl->GetLineText(0).c_str(), 
//                    std::to_string(_fpsValue), std::to_string(_cpuHzTxtCtrl));
}




void SettingsWindow::OnCloseWindow(wxCloseEvent &event)
{
	Show(false);
	event.Veto();
}




void SettingsWindow::OnCancel(wxCommandEvent&)
{
	Show(false);
}



void SettingsWindow::OnOkay(wxCommandEvent&)
{
	UpdateConfigStr();
	SaveSettings();
	Show(false);
}




void SettingsWindow::OnDefault(wxCommandEvent&)
{
	ResetSettings();
}



void SettingsWindow::OnSetRenderPlugin(wxCommandEvent&)
{
	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;
	
	_renderTxtCtrl->Clear();
	*_renderTxtCtrl << dlg.GetPath();
}




void SettingsWindow::OnSetInputPlugin(wxCommandEvent&)
{
	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;

	_inputTxtCtrl->Clear();
	*_inputTxtCtrl << dlg.GetPath();

	
}





void SettingsWindow::OnSetSoundPlugin(wxCommandEvent&)
{

	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;
	
	_soundTxtCtrl->Clear();
	*_soundTxtCtrl << dlg.GetPath();
	
}






void SettingsWindow::OnSetBKGColor(wxCommandEvent&)
{
	ExecuteWxColourDialog(this, &_bkgColor, &_bkgColorStr);
	std::cout << "BKG: " << _bkgColorStr << '\n';
}


void SettingsWindow::OnSetFGColor(wxCommandEvent&)
{
	ExecuteWxColourDialog(this, &_fgColor, &_fgColorStr);
	std::cout << "FG: " << _fgColorStr << '\n';
}






void SettingsWindow::ResetSettings()
{
	// if wxTextCtrl are set  screen refreshes it
	// we don't want to refresh the ones who are already defautl
	const auto SetIfNotEq = [](std::unique_ptr<wxTextCtrl>& tc, const wxString& str) {
		if( tc->GetLineText(0) != str ) {
			tc->Clear();
			*tc << str;
		}
	};

	SetIfNotEq(_cpuHzTxtCtrl, wxString(default_cpu_hz));
	SetIfNotEq(_fpsTxtCtrl, wxString(default_fps));
	SetIfNotEq(_renderTxtCtrl, default_render_full_path);
	SetIfNotEq(_inputTxtCtrl, default_input_full_path);
	SetIfNotEq(_soundTxtCtrl, default_sound_full_path);

	// these are internal variables not shown on screen
	_bkgColor.SetRGB(default_bkg_color);
	_fgColor.SetRGB(default_fg_color);
	_bkgColorStr = default_bkg_color_str;
	_fgColorStr = default_fg_color_str;
}









static void ExecuteWxColourDialog(wxFrame* frame, wxColour* color, std::string* str) 
{
	using std::to_string;
	wxColourData data;
	data.SetColour(*color);
	wxColourDialog dialog(frame, &data);
	if (dialog.ShowModal() == wxID_OK) 
	{
		*color = dialog.GetColourData().GetColour();
		*str = to_string(color->Red());
		*str += 'x';
		*str += to_string(color->Green());
		*str += 'x';
		*str += to_string(color->Blue());
	}
}
