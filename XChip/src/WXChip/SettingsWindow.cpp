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


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cstdlib>
#include <string>


#include <wx/valnum.h>
#include <Utix/Log.h>
#include <Utix/Common.h>
#include <WXChip/SaveList.h>
#include <WXChip/SettingsWindow.h>




wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
EVT_BUTTON(ID_BTN_OK, SettingsWindow::OnOkay)
EVT_BUTTON(ID_BTN_CANCEL, SettingsWindow::OnCancel)
EVT_BUTTON(ID_BTN_DEFAULT, SettingsWindow::OnDefault)
EVT_BUTTON(ID_PLUGRENDIR, SettingsWindow::OnSetRenderPlugin)
EVT_BUTTON(ID_PLUGINDIR, SettingsWindow::OnSetInputPlugin)
EVT_BUTTON(ID_PLUGSNDDIR, SettingsWindow::OnSetSoundPlugin)

wxEND_EVENT_TABLE()


constexpr decltype(SettingsWindow::defaultCpuFreq) SettingsWindow::defaultCpuFreq;
constexpr decltype(SettingsWindow::defaultFPS) SettingsWindow::defaultFPS;


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, 
                  wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	const wxString procPath = utix::GetFullProcDir();
	defaultRender = (procPath + "/bin/plugins/XChipSDLRender");
	defaultInput = (procPath + "/bin/plugins/XChipSDLInput");
	defaultSound = (procPath + "/bin/plugins/XChipSDLSound");

	CreateControls();
	UpdateConfigStr();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}



std::string SettingsWindow::GetDirPath() const
{
	return static_cast<const char*>(_dirPath->GetLineText(0).c_str());
}


int SettingsWindow::GetCPUFreq() const 
{ 
	return std::stoi((const char*)_cpuHz->GetLineText(0).c_str());
}

float SettingsWindow::GetFPS() const 
{ 
	return std::stof((const char*)_fps->GetLineText(0).c_str()); 
}


void SettingsWindow::SetDirPath(const std::string &dirPath)
{
	_dirPath->Clear();
	*_dirPath << dirPath.c_str();
}


void SettingsWindow::SetCPUFreq(const int freq) 
{ 
	_cpuHz->Clear();
	*_cpuHz << freq;

}

void SettingsWindow::SetFPS(const float fps) 
{ 
	_fps->Clear();
	*_fps << fps;
}


void SettingsWindow::UpdateConfigStr()
{
	_configStr = " -FPS ";
	_configStr += (const char*) _fps->GetLineText(0).c_str();
	_configStr += " -CHZ ";
	_configStr += (const char*) _cpuHz->GetLineText(0).c_str();
	_configStr += " -REN ";
	_configStr += (const char*) _plugRenText->GetLineText(0).c_str();
	_configStr += " -INP ";
	_configStr += (const char*) _plugInText->GetLineText(0).c_str();
	_configStr += " -SND ";
	_configStr += (const char*) _plugSndText->GetLineText(0).c_str();
}


void SettingsWindow::CreateControls()
{
	using utix::make_unique;


	_panel = make_unique<wxPanel>(this, wxID_ANY);

	_dirTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Directory: "), 
                                            wxPoint(10,15), wxSize(80,25));

	_dirPath = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, _T(""), 
                                            wxPoint(100,10), wxSize(320,20), wxTE_READONLY);

	// validators 
	wxIntegerValidator<int> cpuFreqValidator(nullptr, wxNUM_VAL_ZERO_AS_BLANK);
	cpuFreqValidator.SetRange(1, 5000);

	wxFloatingPointValidator<float> fpsValidator(2, nullptr, wxNUM_VAL_ZERO_AS_BLANK);
	fpsValidator.SetRange(1.00, 120.00);


	_cpuHzTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Hz: "), 
                                                 wxPoint(220,45), wxSize(60,25));

	_cpuHz = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, defaultCpuFreq, 
                                                  wxPoint(320,40), wxSize(100,20), 0, cpuFreqValidator);

	_fpsTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), 
                                             wxPoint(10,45), wxSize(60,25));

	_fps = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, defaultFPS,
                                               wxPoint(100,40), wxSize(100,20), 0, fpsValidator);



	_plugRenDir = make_unique<wxButton>(_panel.get(), ID_PLUGRENDIR, _T("Render Plugin"),
                                             wxPoint(10, 115), wxSize(140, 33));
	
	_plugRenText = make_unique<wxTextCtrl>(_panel.get(), ID_PLUGRENTEXT, defaultRender,
                                                wxPoint(165, 120), wxSize(200, 20), wxTE_READONLY);
	
	_plugInDir = make_unique<wxButton>(_panel.get(), ID_PLUGINDIR, _T("Input Plugin"),
                                            wxPoint(10, 155), wxSize(140, 33));

	_plugInText = make_unique<wxTextCtrl>(_panel.get(), ID_PLUGINTEXT, defaultInput,
                                           wxPoint(165, 160), wxSize(200, 20), wxTE_READONLY);
		
	_plugSndDir = make_unique<wxButton>(_panel.get(), ID_PLUGSNDDIR, _T("Sound Plugin"),
                                         wxPoint(10, 195), wxSize(140, 33));
	
	_plugSndText = make_unique<wxTextCtrl>(_panel.get(), ID_PLUGSNDTEXT, defaultSound, 
                                           wxPoint(165, 200), wxSize(200, 20), wxTE_READONLY);


	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BTN_OK, _T("Ok"), 
                                           wxPoint(10, 250), wxSize(100, 35));

	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BTN_CANCEL, _T("Cancel"), 
                                               wxPoint(120, 250), wxSize(100, 35));

	_buttonDefault = make_unique<wxButton>(_panel.get(), ID_BTN_DEFAULT, _T("Default"), 
                                                wxPoint(230,250), wxSize(100, 35));
	
}

void SettingsWindow::SaveSettings()
{
//	saveDirectory((const char*)_dirTxtCtrl->GetLineText(0).c_str(), 
//                    std::to_string(_fpsValue), std::to_string(_cpuHz));
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
	ResetTextControls();
}

void SettingsWindow::OnSetRenderPlugin(wxCommandEvent&)
{
	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;
	
	_plugRenText->Clear();
	*_plugRenText << dlg.GetPath();
}

void SettingsWindow::OnSetInputPlugin(wxCommandEvent&)
{
	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;

	_plugInText->Clear();
	*_plugInText << dlg.GetPath();

	
}

void SettingsWindow::OnSetSoundPlugin(wxCommandEvent&)
{

	wxFileDialog dlg(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if (dlg.ShowModal() == wxID_CANCEL)
		return;
	
	_plugSndText->Clear();
	*_plugSndText << dlg.GetPath();
	
}



void SettingsWindow::ResetTextControls()
{
	_fps->Clear();
	_cpuHz->Clear();
	_plugRenText->Clear();
	_plugInText->Clear();
	_plugSndText->Clear();

	*_fps << defaultFPS;
	*_cpuHz << defaultCpuFreq;
	*_plugRenText << defaultRender;
	*_plugInText << defaultInput;
	*_plugSndText << defaultSound;
}


