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


#include <WXChip/SaveList.h>
#include <WXChip/SettingsWindow.h>








wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
EVT_BUTTON(ID_BTN_OK, SettingsWindow::OnOkay)
EVT_BUTTON(ID_BTN_CANCEL, SettingsWindow::OnCancel)
EVT_BUTTON(ID_BTN_DEFAULT, SettingsWindow::OnDefault)
wxEND_EVENT_TABLE()


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{

	CreateControls();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void SettingsWindow::SetDirPath(const std::string &dirPath)
{
	_dirPath->Clear();
	*_dirPath << dirPath.c_str();
}

void SettingsWindow::CreateControls()
{
	using xchip::utility::make_unique;
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_dirText = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Dir Path: "), 
                                             wxPoint(10,15), wxSize(150,25));

	_dirPath = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", 
                                           wxPoint(100,10), wxSize(320,20),wxTE_READONLY);

	wxFloatingPointValidator<float> _val(2,&_fpsValue,wxNUM_VAL_ZERO_AS_BLANK);
	_val.SetRange(1.0,60.0);
	_fpsText = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), 
                                             wxPoint(10,40), wxSize(150,25));

	_fps = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, _T("60"), 
                                       wxPoint(100,40), wxSize(100,20),0,_val);
	_cpuText = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Freq: "), 
                                             wxPoint(220,40), wxSize(150,25));

	wxIntegerValidator<unsigned int>
	val(&_cpuFreq, wxNUM_VAL_ZERO_AS_BLANK);
	val.SetRange(1, 1000);

	_cpu = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, _T("60"), 
                                       wxPoint(320,40), wxSize(100,20), 0, val);

	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BTN_OK, _T("Ok"), 
                                          wxPoint(10, 150), wxSize(100,25));

	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BTN_CANCEL, _T("Cancel"), 
                                              wxPoint(120, 150), wxSize(100,25));

	_buttonDefault = make_unique<wxButton>(_panel.get(), ID_BTN_DEFAULT, _T("Default"), 
                                               wxPoint(230,150), wxSize(100,25));

}

void SettingsWindow::SaveSettings()
{
	saveDirectory((const char*)_dirPath->GetLineText(0).c_str(), 
                      std::to_string(_fpsValue), std::to_string(_cpuFreq));
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
	
	if(atoi(_cpu->GetLineText(0)) < 60)
	{
		wxMessageBox( "CPU Frequency must be a value between 60 and 1000",
					 "Input Error", wxOK | wxICON_ERROR );
		return;
	}
	// save settings
	SaveSettings();
	Show(false);
}

void SettingsWindow::OnDefault(wxCommandEvent&)
{
	_fps->Clear();
	_cpu->Clear();
	*_fps << "60";
	*_cpu << "60";
}

/*
std::string SettingsWindow::RomPath() const
{
	wxString val = _romPath->GetLineText(0);
	return std::string(val.c_str());
}

std::string SettingsWindow::CPUFreq() const
{
	wxString val = _cpu->GetLineText(0);
	return std::string(val.c_str());
}

std::string SettingsWindow::FPS() const
{
	wxString val = _fps->GetLineText(0);
	return std::string(val.c_str());
}
*/


std::string SettingsWindow::GetDirPath() const
{
	return static_cast<const char*>(_dirPath->GetLineText(0).c_str());
}
