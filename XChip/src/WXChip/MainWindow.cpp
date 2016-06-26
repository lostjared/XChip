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

#ifdef _WIN32
#include <stdlib.h>
#include <windows.h>
#include <WXChip/dirent.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#endif


#include <iostream>
#include <fstream>
#include <regex>
#include <stdexcept>

#include <Utix/ScopeExit.h>
#include <Utix/Common.h>
#include <Utix/CliOpts.h>
#include <Utix/Log.h>
#include <Utix/Memory.h>

#include <WXChip/Main.h>
#include <WXChip/SaveList.h>
#include <WXChip/MainWindow.h>


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MENU(ID_MENU_BAR_LOAD_ROM, MainWindow::OnMenuBarLoadRom)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_BUTTON(ID_BUTTON_LOAD_ROM, MainWindow::OnButtonLoadRom)
EVT_BUTTON(ID_BUTTON_SELECT_DIR, MainWindow::OnButtonSelectDir)
EVT_BUTTON(ID_BUTTON_SETTINGS, MainWindow::OnButtonSettings)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	using utix::make_unique;
	utix::Log("Constructing WXChip MainWindow");
	
	ComputeEmuAppPath();
	CreateControls();

	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_MENU_BAR_LOAD_ROM, "&Load Rom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuHelp = make_unique<wxMenu>();
	menuHelp->Append(wxID_ABOUT);


	auto menuBar = make_unique<wxMenuBar>();

	if (!menuBar->Append(menuFile.get(), "&File") 
		|| !menuBar->Append(menuHelp.release(), "&Help")) {
		throw std::runtime_error("could not append a menu into wxMenuBar");
	}

	menuFile.release();
	SetMenuBar(menuBar.release());

	CreateStatusBar();
	SetStatusText("Welcome to WXChip");
}


MainWindow::~MainWindow()
{
	utix::Log("Destroying MainWindow...");
	StopEmulator();
	Destroy();
}





void MainWindow::OnExit(wxCommandEvent&)
{
	Close(true);
}



void MainWindow::OnAbout(wxCommandEvent&)
{
	wxMessageBox("WXChip - wxWidgets GUI for XChip",
		"About WXChip", wxOK | wxICON_INFORMATION);
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
	auto m_lbox = static_cast<wxListBox*>(event.GetEventObject());
	int item = m_lbox->HitTest(event.GetPosition());

	if (item != wxNOT_FOUND)
	{
		wxString str = m_lbox->GetString(item);
		_romPath = _settingsWin->GetDirPath();
#ifdef _WIN32
		_romPath += '\\';
#elif defined(__APPLE__) || defined(__linux__)
		_romPath += '/';
#endif
		_romPath += str.c_str();

		utix::Log("Start Rom At Path: %s", _romPath.c_str());

		StartEmulator();
		
	}
}


void MainWindow::OnButtonSettings(wxCommandEvent&)
{
	_settingsWin->Show(true);
}



void MainWindow::OnButtonLoadRom(wxCommandEvent&)
{
	utix::Log("Starting Rom...");
	StartEmulator();
}


void MainWindow::OnButtonSelectDir(wxCommandEvent&)
{
	wxDirDialog dlg(NULL, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;


	wxString value = dlg.GetPath();
	LoadList(std::string(value.c_str()));
}



void MainWindow::OnMenuBarLoadRom(wxCommandEvent&)
{
	StopEmulator();

	wxFileDialog openDialog(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openDialog.ShowModal() == wxID_OK) 
	{
		_romPath = openDialog.GetPath().c_str();
		utix::Log("Selected File: %s", _romPath.c_str());
		StartEmulator();
	}
}



void MainWindow::StartEmulator()
{
	StopEmulator();
	if(!_process.Run(_emuApp + "-ROM \"" + _romPath + "\" " + _settingsWin->GetArguments()))
		throw std::runtime_error(utix::GetLastLogError());
}


void MainWindow::StopEmulator()
{
	if (_process.IsRunning())
	{
		_process.Terminate();
	}
}




void MainWindow::CreateControls()
{
	using utix::make_unique;


	wxArrayString strings;

	_panel = make_unique<wxPanel>(this, wxID_ANY);

	_text = make_unique<wxStaticText>(_panel.get(), ID_TEXT, _T("Chip8 Roms"), 
                                           wxPoint(10, 10), wxSize(100, 25));

	_listBox = make_unique<wxListBox>(_panel.get(), ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), 
                                           strings, wxLB_SINGLE);


	_listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);


	_buttonLoadRom = make_unique<wxButton>(_panel.get(), ID_BUTTON_LOAD_ROM, _T("Load Rom"), 
                                           wxPoint(10, 400), wxSize(100, 35));


	_buttonSelectDir = make_unique<wxButton>(_panel.get(), ID_BUTTON_SELECT_DIR, _T("Select Directory"), 
                                           wxPoint(120, 400), wxSize(110, 35));


	_buttonSettings = make_unique<wxButton>(_panel.get(), ID_BUTTON_SETTINGS, _T("Settings"), 
                                                   wxPoint(240, 400), wxSize(100, 35));


	_settingsWin = make_unique<SettingsWindow>("WXChip - Settings", wxPoint(150, 150));

}








void MainWindow::LoadList(const std::string &dirPath)
{
	using namespace utix;

	if (dirPath == "nopath" || dirPath == _settingsWin->GetDirPath())
		return;

	DIR *dir = opendir(dirPath.c_str());

	if (dir == nullptr)
	{
		LogError("Error could not open directory.");
		return;
	}

	// close the dir in every exit path from this function
	const auto cleanup = make_scope_exit([&dir]() noexcept { closedir(dir); });	
	wxArrayString dirFiles;
	dirent *e;

	while ((e = readdir(dir)))
	{
		if (e->d_type == DT_REG)
		{
			std::string file = e->d_name;
			std::regex exp1("ch8$", std::regex_constants::icase);
			std::regex exp2("([0-9a-zA-Z_\\ ]+)", std::regex_constants::icase);
			// if found in 1 regex search, avoid doing the other search
			bool isTag = std::regex_search(file, exp1) || std::regex_match(file, exp2);
			if (isTag) 
				dirFiles.Add(wxString(e->d_name));
		}
	}

	if(!dirFiles.IsEmpty())
	{
		_listBox->Clear();
		_listBox->InsertItems(dirFiles,0);
		_settingsWin->SetDirPath(dirPath);
	}
}



void MainWindow::ComputeEmuAppPath()
{
#ifdef _WIN32
	constexpr const char* const finalEmuAppPath = "\\bin\\EmuApp.exe";
#elif defined(__APPLE__) || defined(__linux__)
	constexpr const char* const finalEmuAppPath = "/bin/EmuApp";
#endif

	_emuApp = utix::GetFullProcDir() + finalEmuAppPath;

	if (!std::ifstream(_emuApp).good())
		throw std::runtime_error("Could not find EmuApp executable!");

	_emuApp.insert(0, "\"");
	_emuApp += "\" ";

	utix::Log("_emuApp after compute: %s", _emuApp.c_str());

}



