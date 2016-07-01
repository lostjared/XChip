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
#include <cstring>
#include <string>

#include <wx/valnum.h>
#include <Utix/Log.h>
#include <Utix/Assert.h>
#include <Utix/Common.h>
#include <WXChip/Dialog.h>
#include <WXChip/SettingsWindow.h>


// local functions declarations
namespace {
static void ExecuteWxColourDialog(wxFrame* const frame, wxColour& color, wxString& str);
static void ExecuteWxFileDialog(wxFrame* const frame, wxTextCtrl& text_ctrl);
}





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


constexpr const char* const SettingsWindow::default_cpu_hz;
constexpr const char* const SettingsWindow::default_fps;
constexpr const unsigned long SettingsWindow::default_bkg_color;
constexpr const unsigned long SettingsWindow::default_fg_color;
constexpr const char* const SettingsWindow::default_bkg_color_str;
constexpr const char* const SettingsWindow::default_fg_color_str;
constexpr const char* const SettingsWindow::default_render_relative_path;
constexpr const char* const SettingsWindow::default_input_relative_path;
constexpr const char* const SettingsWindow::default_sound_relative_path;


SettingsWindow::SettingsWindow(wxFrame* parent, const wxString &title, const wxPoint &pos)
	: wxFrame(parent, wxID_ANY, title, pos, wxSize(450, 360), 
                  wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX),
	m_bkgColor(default_bkg_color),
	m_fgColor(default_fg_color),
	m_bkgColorStr(default_bkg_color_str),
	m_fgColorStr(default_fg_color_str)

{
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
	CreateControls();
	UpdateConfigStr();
}

// gets / sets



wxString SettingsWindow::GetDirPath() const
{
	return m_romsDirTxtCtrl->GetLineText(0);
}


void SettingsWindow::SetDirPath(const wxString& dirPath)
{
	m_romsDirTxtCtrl->Clear();
	*m_romsDirTxtCtrl << dirPath;
}


// functionality 

void SettingsWindow::CreateControls()
{
	m_panel = utix::make_unique<wxPanel>(this, wxID_ANY);
	EvaluateDefaultPlugins();
	CreateTexts();
	CreateButtons();
}


void SettingsWindow::CreateTexts()
{
	using utix::make_unique;
	// validators 
	wxIntegerValidator<int> cpuHzValidador(nullptr, wxNUM_VAL_ZERO_AS_BLANK);
	wxFloatingPointValidator<float> fpsValidador(2, nullptr, wxNUM_VAL_ZERO_AS_BLANK);

	cpuHzValidador.SetRange(1, 5000);
	fpsValidador.SetRange(1.00, 120.00);

	// static texts
	m_cpuHzTxt = make_unique<wxStaticText>(m_panel.get(), ID_CPU_HZ_TEXT,_T("CPU Hz: "), 
                                                 wxPoint(220,45), wxSize(60,25));

	m_fpsTxt = make_unique<wxStaticText>(m_panel.get(), ID_FPS_TEXT,_T("FPS: "), 
                                         wxPoint(10,45), wxSize(60,25));

	m_romsDirTxt = make_unique<wxStaticText>(m_panel.get(), ID_ROMS_DIR_TEXT,_T("Directory: "), 
	                                        wxPoint(10,15), wxSize(80,25));

	// text controls
	m_fpsTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_FPS_TEXT_CTRL, default_fps,
                                           wxPoint(100,40), wxSize(100,20), 0, fpsValidador);

	m_cpuHzTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_CPU_HZ_TEXT_CTRL, default_cpu_hz, 
                                                  wxPoint(320,40), wxSize(100,20), 0, cpuHzValidador);

	m_romsDirTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_ROMS_DIR_TEXT_CTRL, _T(""), 
                                               wxPoint(100,10), wxSize(320,20), wxTE_READONLY);

	m_renderTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_RENDER_TEXT_CTRL, default_render_full_path,
                                                wxPoint(165, 120), wxSize(200, 20), wxTE_READONLY);

	m_inputTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_INPUT_TEXT_CTRL, default_input_full_path,
                                           wxPoint(165, 160), wxSize(200, 20), wxTE_READONLY);

	m_soundTxtCtrl = make_unique<wxTextCtrl>(m_panel.get(), ID_SOUND_TEXT_CTRL, default_sound_full_path, 
                                           wxPoint(165, 200), wxSize(200, 20), wxTE_READONLY);
}





void SettingsWindow::CreateButtons()
{
	using utix::make_unique;

	m_buttonBKGColor= make_unique<wxButton>(m_panel.get(), ID_BUTTON_BKG_COLOR, _T("Background Color"),
                                                wxPoint(10, 75), wxSize(145, 35));

	m_buttonFGColor= make_unique<wxButton>(m_panel.get(), ID_BUTTON_FG_COLOR, _T("Frontground Color"),
                                            wxPoint(160, 75), wxSize(145, 35));

	m_buttonRender = make_unique<wxButton>(m_panel.get(), ID_BUTTON_RENDER, _T("Render Plugin"),
                                             wxPoint(10, 115), wxSize(140, 33));
	
	m_buttonInput = make_unique<wxButton>(m_panel.get(), ID_BUTTON_INPUT, _T("Input Plugin"),
                                            wxPoint(10, 155), wxSize(140, 33));

	m_buttonSound = make_unique<wxButton>(m_panel.get(), ID_BUTTON_SOUND, _T("Sound Plugin"),
                                         wxPoint(10, 195), wxSize(140, 33));

	m_buttonOk = make_unique<wxButton>(m_panel.get(), ID_BUTTON_OK, _T("Ok"), 
	                                       wxPoint(10, 250), wxSize(100, 35));

	m_buttonCancel = make_unique<wxButton>(m_panel.get(), ID_BUTTON_CANCEL, _T("Cancel"), 
	                                           wxPoint(120, 250), wxSize(100, 35));

	m_buttonDefault = make_unique<wxButton>(m_panel.get(), ID_BUTTON_DEFAULT, _T("Default"), 
	                                            wxPoint(230,250), wxSize(100, 35));
}




void SettingsWindow::EvaluateDefaultPlugins()
{
	const wxString procDirPath = utix::GetFullProcDir();
	if(procDirPath.empty() == false) 
	{
		default_render_full_path = (procDirPath + default_render_relative_path);
		default_input_full_path = (procDirPath + default_input_relative_path);
		default_sound_full_path = (procDirPath + default_sound_relative_path);
	}
	else 
	{
		WarningDlg(this, "Could not get WXChip process's path! Please, set Plugins paths manualy");
	}
}



void SettingsWindow::UpdateConfigStr()
{
	m_configStr = "";
	const auto AddArg = [this](const char* opt, const char* arg) {
		(((m_configStr  += opt) += ' ') += arg) += ' ';
	};
	AddArg("-CHZ", m_cpuHzTxtCtrl->GetLineText(0).c_str());
	AddArg("-FPS", m_fpsTxtCtrl->GetLineText(0).c_str());
	AddArg("-REN", m_renderTxtCtrl->GetLineText(0).c_str());
	AddArg("-INP", m_inputTxtCtrl ->GetLineText(0).c_str());
	AddArg("-SND", m_soundTxtCtrl ->GetLineText(0).c_str());
	AddArg("-COL", m_fgColorStr.c_str());
	AddArg("-BKG", m_bkgColorStr.c_str());
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

	SetIfNotEq(m_cpuHzTxtCtrl, wxString(default_cpu_hz));
	SetIfNotEq(m_fpsTxtCtrl, wxString(default_fps));
	SetIfNotEq(m_renderTxtCtrl, default_render_full_path);
	SetIfNotEq(m_inputTxtCtrl, default_input_full_path);
	SetIfNotEq(m_soundTxtCtrl, default_sound_full_path);

	// these variables are not shown on screen
	// no bother cheking it
	m_bkgColor.SetRGB(default_bkg_color);
	m_fgColor.SetRGB(default_fg_color);
	m_bkgColorStr = default_bkg_color_str;
	m_fgColorStr = default_fg_color_str;
}



void SettingsWindow::RestoreSettings()
{
	// this is not the best solution,
	// as directory names can cause trouble
	// so for now this code is enabled until i have time
	// to develop a better way so
	// TODO: create internal variables to represent 
	// plugin paths and fps/hz values, so its likely
	// to be faster and nicier to cancel the modifications

	// this will find the old wxTextCtrl value in the m_configStr
	// if the user modified the wxTextCtrl but the changes were not
	// saved on m_configStr, this will get the value on m_configStr
	// and set the wxTextCtrl

	const auto RestoreIfMod = [this](const char* opt, std::unique_ptr<wxTextCtrl>& tc) {
		auto idx = m_configStr.find(opt);
		if(idx != std::string::npos) {
			idx += std::strlen(opt)+1;
			auto idxEnd = m_configStr.find('-', idx);
			if( idxEnd == std::string::npos )
				idxEnd = m_configStr.size();

			const auto str =  m_configStr.substr(idx, (idxEnd - idx) - 1);
			if( tc->GetLineText(0) != str ) {
				tc->Clear();
				*tc << str;
			}
		}
	};

	RestoreIfMod("-REN", m_renderTxtCtrl);
	RestoreIfMod("-INP", m_inputTxtCtrl);
	RestoreIfMod("-SND", m_soundTxtCtrl);
	RestoreIfMod("-CHZ", m_cpuHzTxtCtrl);
	RestoreIfMod("-FPS", m_fpsTxtCtrl);
}







// GUI control 

void SettingsWindow::OnCloseWindow(wxCloseEvent &event)
{
	Show(false);
	event.Veto();
}




void SettingsWindow::OnCancel(wxCommandEvent&)
{
	RestoreSettings();
	Show(false);
}



void SettingsWindow::OnOkay(wxCommandEvent&)
{
	UpdateConfigStr();
	Show(false);
}




void SettingsWindow::OnDefault(wxCommandEvent&)
{
	ResetSettings();
}



void SettingsWindow::OnSetRenderPlugin(wxCommandEvent&)
{
	ExecuteWxFileDialog(this, *m_renderTxtCtrl);
}


void SettingsWindow::OnSetInputPlugin(wxCommandEvent&)
{
	ExecuteWxFileDialog(this, *m_inputTxtCtrl);
}


void SettingsWindow::OnSetSoundPlugin(wxCommandEvent&)
{
	ExecuteWxFileDialog(this, *m_soundTxtCtrl);
}


void SettingsWindow::OnSetBKGColor(wxCommandEvent&)
{
	ExecuteWxColourDialog(this, m_bkgColor, m_bkgColorStr);
}


void SettingsWindow::OnSetFGColor(wxCommandEvent&)
{
	ExecuteWxColourDialog(this, m_fgColor, m_fgColorStr);
}




namespace {
// local functions definitions





static void ExecuteWxColourDialog(wxFrame* const frame, wxColour& color, wxString& str)
{
	ASSERT_MSG(frame, "ExecuteWxColourDialog can't have null frame");
	using std::to_string;

	if (ColourDlg(frame, color))
	{
		str = to_string(color.Red());
		str += 'x';
		str += to_string(color.Green());
		str += 'x';
		str += to_string(color.Blue());
	}

}




static void ExecuteWxFileDialog(wxFrame* const frame, wxTextCtrl& text_ctrl) 
{
	ASSERT_MSG(frame, "ExecuteWxFileDialog can't have null frame");
	const wxString filepath = FileDlg(frame, "Select File", "All Files (*)|*");
	
	if (filepath.empty() == false) {
		text_ctrl.Clear();
		text_ctrl << filepath;
	}

}










}
