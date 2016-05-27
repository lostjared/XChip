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


#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cstdlib>
#include <string>


#include <wx/valnum.h>
#include <XChip/Utility/Log.h>
#include <WXChip/SaveList.h>
#include <WXChip/SettingsWindow.h>




wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
EVT_BUTTON(ID_BTN_OK, SettingsWindow::OnOkay)
EVT_BUTTON(ID_BTN_CANCEL, SettingsWindow::OnCancel)
EVT_BUTTON(ID_BTN_DEFAULT, SettingsWindow::OnDefault)
wxEND_EVENT_TABLE()


constexpr decltype(SettingsWindow::defaultCpuFreq) SettingsWindow::defaultCpuFreq;
constexpr decltype(SettingsWindow::defaultFPS) SettingsWindow::defaultFPS;


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, 
                  wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{

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
	_configStr += " -RES ";
	_configStr += (const char*) _emuRes->GetStringSelection().c_str();
	_configStr += " ";
}


void SettingsWindow::CreateControls()
{
	using xchip::utility::make_unique;


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


	_cpuHzTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Freq: "), 
                                                 wxPoint(220,40), wxSize(60,25));

	_cpuHz = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, defaultCpuFreq, 
                                                  wxPoint(320,40), wxSize(100,20), 0, cpuFreqValidator);

	_fpsTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), 
                                             wxPoint(10,40), wxSize(60,25));

	_fps = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, defaultFPS,
                                               wxPoint(100,40), wxSize(100,20), 0, fpsValidator);

	_emuResText = make_unique<wxStaticText>(_panel.get(), ID_TEXT4, _T("Resolution: "), 
                                                wxPoint(10, 70), wxSize(60, 25));

	
	wxString sizeChoices[] = { "320x240", "640x480", "1280x720", "1920x1080", "FULLSCREEN" };
	

	_emuRes = make_unique<wxComboBox>(_panel.get(), ID_RES, _T("320x240"), 
                                              wxPoint(100, 70), wxSize(200,25), 5, sizeChoices, wxCB_READONLY);


	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BTN_OK, _T("Ok"), 
                                           wxPoint(10, 150), wxSize(100,25));

	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BTN_CANCEL, _T("Cancel"), 
                                               wxPoint(120, 150), wxSize(100,25));

	_buttonDefault = make_unique<wxButton>(_panel.get(), ID_BTN_DEFAULT, _T("Default"), 
                                                wxPoint(230,150), wxSize(100,25));


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



void SettingsWindow::ResetTextControls()
{
	_fps->Clear();
	_cpuHz->Clear();
	*_fps << defaultFPS;
	*_cpuHz << defaultCpuFreq;
	_emuRes->SetSelection(0);
}


