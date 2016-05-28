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

#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_
#include <string>

#include <wx/frame.h>
#include <XChip/Utility/Process.h>
#include <WXChip/SettingsWindow.h>

class MainWindow final : public wxFrame
{
public:
	enum { ID_LoadRom, ID_TEXT, ID_LISTBOX, ID_STARTROM, ID_LOADROM, ID_EMUSET, ID_LOADROMDIR };
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();


private:
	void OnExit(wxCommandEvent& ev);
	void OnAbout(wxCommandEvent& ev);
	void OnLDown(wxMouseEvent& ev);
	void OnStartRom(wxCommandEvent& ev);
	void OnLoadSettings(wxCommandEvent& ev);
	void OnLoadRomDir(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);
	void StartEmulator();
	void StopEmulator();	
	void CreateControls();
	void LoadList(const std::string &dirPath);
	void ComputeEmuAppPath();
	static std::string GetFullWD();


	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;
	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::unique_ptr<SettingsWindow> _settingsWin;
	std::string _emuApp;
	std::string _romPath;
	xchip::utility::Process _process;
	wxDECLARE_EVENT_TABLE();
};





#endif
