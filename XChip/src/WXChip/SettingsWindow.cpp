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


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, 
                  wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{

	CreateControls();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}



std::string SettingsWindow::GetEmuConfigStr() const
{
	std::string config;
	config = "-FPS ";
	config += (const char*) _fpsTxtCtrl->GetLineText(0).c_str();
	config += " -CFQ ";
	config += (const char*) _cpuFreqTxtCtrl->GetLineText(0).c_str();

	return config;
}


void SettingsWindow::SetDirPath(const std::string &dirPath)
{
	_dirTxtCtrl->Clear();
	*_dirTxtCtrl << dirPath.c_str();
}


void SettingsWindow::SetCPUFreq(const int freq) 
{ 
	_cpuFreqTxtCtrl->Clear();
	*_cpuFreqTxtCtrl << freq;
	_cpuFreq = freq;	
}

void SettingsWindow::SetSoundFreq(const float freq) 
{
	_soundFreq = freq; 
}

void SettingsWindow::SetFPS(const float fps) 
{ 
	_fpsTxtCtrl->Clear();
	*_fpsTxtCtrl << fps;
	_fps = fps;
}




void SettingsWindow::CreateControls()
{
	using xchip::utility::make_unique;


	_panel = make_unique<wxPanel>(this, wxID_ANY);

	_dirTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Roms Directory: "), 
                                             wxPoint(10,15), wxSize(150,25));

	_dirTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", 
                                              wxPoint(100,10), wxSize(320,20),wxTE_READONLY);

	// validators 
	wxIntegerValidator<int> cpuFreqValidator(&_cpuFreq, wxNUM_VAL_ZERO_AS_BLANK);
	cpuFreqValidator.SetRange(1, 5000);

	wxFloatingPointValidator<float> fpsValidator(2, &_fps, wxNUM_VAL_ZERO_AS_BLANK);
	fpsValidator.SetRange(1.0f, 120.0f);


	_cpuFreqTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Freq: "), 
                                             wxPoint(220,40), wxSize(150,25));

	_cpuFreqTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, _T("60"), 
                                               wxPoint(320,40), wxSize(100,20), 0, cpuFreqValidator);

	_fpsTxt = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), 
                                             wxPoint(10,40), wxSize(150,25));

	_fpsTxtCtrl = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, _T("60"), 
                                               wxPoint(100,40), wxSize(100,20),0, fpsValidator);


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
//                    std::to_string(_fpsValue), std::to_string(_cpuFreq));
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
	using namespace xchip::utility;
	using namespace xchip::utility::literals;

	// transfer contents on TextControls to 
	// SettingsWindow member variables
	_fpsTxtCtrl->GetValidator()->TransferFromWindow();
	_cpuFreqTxtCtrl->GetValidator()->TransferFromWindow();
	SaveSettings();
	Show(false);
}

void SettingsWindow::OnDefault(wxCommandEvent&)
{
	_fpsTxtCtrl->Clear();
	_cpuFreqTxtCtrl->Clear();
	*_fpsTxtCtrl << "60";
	*_cpuFreqTxtCtrl << "60";
}

std::string SettingsWindow::GetDirPath() const
{
	return static_cast<const char*>(_dirTxtCtrl->GetLineText(0).c_str());
}
