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


#include <fstream>
#include <regex>
#include <stdexcept>



#include <Utix/Assert.h>
#include <Utix/ScopeExit.h>
#include <Utix/Common.h>
#include <Utix/CliOpts.h>
#include <Utix/Log.h>
#include <Utix/Memory.h>

#include <WXChip/Dialog.h>
#include <WXChip/Main.h>
#include <WXChip/SaveList.h>
#include <WXChip/MainWindow.h>



// local functions declarations
namespace {
static void FillRomPath(const wxString& dirPath, const wxString& filename, wxString& dest);
inline void FillRomPath(const wxString& fullPath, wxString& dest);
static bool LoadListBox(wxFrame* const parent, const wxString& path, wxListBox& lbox);
inline std::string ComputeEmuAppCommand(const wxString& emuAppPath, const wxString& rom, const wxString& cliArgs);
inline const char* ToCStr(const wxString& wxstr);
}


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MENU(ID_MENU_BAR_LOAD_ROM, MainWindow::OnMenuBarLoadRom)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_BUTTON(ID_BUTTON_LOAD_ROM, MainWindow::OnButtonLoadRom)
EVT_BUTTON(ID_BUTTON_SELECT_DIR, MainWindow::OnButtonSelectDir)
EVT_BUTTON(ID_BUTTON_SETTINGS, MainWindow::OnButtonSettings)
EVT_CLOSE(MainWindow::OnClose)
wxEND_EVENT_TABLE()




constexpr const char* const MainWindow::default_emuapp_relative_path;




MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	utix::Log("Constructing WXChip MainWindow");
	CreateStatusBar();
	SetStatusText("Welcome to WXChip");
	ComputeEmuAppPath();
	CreateControls();
}


MainWindow::~MainWindow()
{
	utix::Log("Destroying MainWindow...");
	StopEmulator();
}



void MainWindow::CreateControls()
{
	using utix::make_unique;

	CreateMenuBar();

	m_panel = make_unique<wxPanel>(this, wxID_ANY);

	m_settingsWin = make_unique<SettingsWindow>(this, "WXChip - Settings", wxPoint(150, 150));


	m_romsTxt = make_unique<wxStaticText>(m_panel.get(), ID_ROMS_TEXT, _T("Roms"), 
                                          wxPoint(10, 10), wxSize(100, 25));

	m_listBox = make_unique<wxListBox>(m_panel.get(), ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), 
                                       0, nullptr, wxLB_SINGLE);	
	m_listBox->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainWindow::OnListBoxMouseClick), nullptr, this);
	m_listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnListBoxMouseClick), nullptr, this);


	m_buttonLoadRom = make_unique<wxButton>(m_panel.get(), ID_BUTTON_LOAD_ROM, _T("Load Rom"), 
                                           wxPoint(10, 400), wxSize(100, 35));

	m_buttonSelectDir = make_unique<wxButton>(m_panel.get(), ID_BUTTON_SELECT_DIR, _T("Select Directory"), 
                                           wxPoint(120, 400), wxSize(110, 35));

	m_buttonSettings = make_unique<wxButton>(m_panel.get(), ID_BUTTON_SETTINGS, _T("Settings"), 
                                                   wxPoint(240, 400), wxSize(100, 35));
}





void MainWindow::CreateMenuBar()
{
	using utix::make_unique;
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
}






void MainWindow::StartEmulator()
{
	StopEmulator();
	if(m_romPath.empty() == false) 
	{
		utix::Log("Start Rom At Path: %s", ToCStr(m_romPath));
		if(!m_process.Run(ComputeEmuAppCommand(m_emuAppPath, m_romPath, m_settingsWin->GetArguments())))
			throw std::runtime_error(utix::GetLastLogError());
	}
	else {
		InformationDlg(this, "No ROM Selected.");
	}
}




void MainWindow::StopEmulator()
{
	if (m_process.IsRunning())
		m_process.Terminate();
}







void MainWindow::ComputeEmuAppPath()
{
	const auto procDir = utix::GetFullProcDir();
	if( procDir.empty() )
		throw std::runtime_error("Could not get WXChip process path");

	m_emuAppPath = procDir + default_emuapp_relative_path;
	if (!std::ifstream(ToCStr(m_emuAppPath)).good())
		throw std::runtime_error("Could not find EmuApp executable!");

	// insert quotes around the computed path
	m_emuAppPath.insert(0, "\"");
	m_emuAppPath += "\"";

	utix::Log("m_emuAppPath after compute: %s", ToCStr(m_emuAppPath));
}







void MainWindow::OnExit(wxCommandEvent&)
{
	Close(true);
}


void MainWindow::OnClose(wxCloseEvent&)
{	
	m_settingsWin.release()->Destroy();
	m_buttonSettings.release()->Destroy();
	m_buttonSelectDir.release()->Destroy();
	m_buttonLoadRom.release()->Destroy();
	m_listBox.release()->Destroy();
	m_romsTxt.release()->Destroy();
	m_panel.release()->Destroy();
	Destroy();
}



void MainWindow::OnAbout(wxCommandEvent&)
{
	InformationDlg(this, "WXChip - wxWidgets GUI for XChip");
}



void MainWindow::OnListBoxMouseClick(wxMouseEvent& event)
{
	auto lbox = static_cast<wxListBox*>(event.GetEventObject());
	int item = lbox->HitTest(event.GetPosition());

	if (item != wxNOT_FOUND) 
	{
		lbox->SetSelection(item);
		FillRomPath(m_settingsWin->GetDirPath(), lbox->GetString(item),m_romPath);
		if (event.ButtonDClick())
			this->StartEmulator();
	}
}




void MainWindow::OnButtonSettings(wxCommandEvent&)
{
	m_settingsWin->Show(true);
}




void MainWindow::OnButtonLoadRom(wxCommandEvent&)
{
	StartEmulator();
}




void MainWindow::OnButtonSelectDir(wxCommandEvent&)
{
	wxString path = DirectoryDlg(this, "Choose Roms Directory");
	if(path.empty() == false && path != m_settingsWin->GetDirPath()) {
		if(LoadListBox(this, path, *m_listBox))
			m_settingsWin->SetDirPath(std::move(path));
	}
}




void MainWindow::OnMenuBarLoadRom(wxCommandEvent&)
{
	const wxString path = FileDlg(this, "Select Rom", "All Files (*)|*");
	if (path.empty() == false)
	{
		FillRomPath(path, m_romPath);
		utix::Log("Selected File: %s", ToCStr(m_romPath));
		StartEmulator();
	}

}




 // local functiosn definitions
namespace {


static void FillRomPath(const wxString& dirPath, const wxString& filename, wxString& dest)
{
#ifdef _WIN32
		constexpr char dirSlash =  '\\';
#elif defined(__APPLE__) || defined(__linux__)
		constexpr char dirSlash = '/';
#endif
	((dest = '\"') += dirPath);

	// check if dirPath had the last dirSlash
	const auto slashIdx =  dest.size() - 1;
	if(dest.at(slashIdx) != dirSlash)
		dest += dirSlash;

	(dest += filename) += '\"';
}



inline void FillRomPath(const wxString& fullPath, wxString& dest)
{
	((dest = '\"') += fullPath) += '\"';
}




static bool LoadListBox(wxFrame* const parent, const wxString& path, wxListBox& lbox)
{
	using namespace utix;
	
	errno = 0;
	DIR* const dir = opendir(path.c_str());

	if( dir == nullptr ) 
	{
		const auto errno_code = errno;
		ErrorDlg(parent, "Error opening \"" + path +"\": " + strerror(errno_code));
		return false;
	}

	const auto dir_cleanup = MakeScopeExit([dir]()noexcept { closedir(dir); });
	
	wxArrayString dirFiles;
	dirent *e;

	while ((e = readdir(dir)) != nullptr)
	{
		if (e->d_type == DT_REG)
		{
			std::string file = e->d_name;
			std::regex exp1("ch8$", std::regex_constants::icase);
			std::regex exp2("([0-9a-zA-Z_\\ ]+)", std::regex_constants::icase);
			if( std::regex_search(file, exp1) || std::regex_match(file, exp2) )
				dirFiles.Add(wxString(e->d_name));
		}
	}


	if(!dirFiles.IsEmpty()) 
	{
		lbox.Clear();
		lbox.InsertItems(dirFiles,0);
		return true;
	}

	WarningDlg(parent, "Couldn't find any \'ch8\' files in this directory");
	return false;
}




inline std::string ComputeEmuAppCommand(const wxString& emuAppPath, const wxString& rom, const wxString& cliArgs)
{
	return static_cast<const char*>((emuAppPath + " -ROM " + rom + ' ' + cliArgs).c_str());
}


inline const char* ToCStr(const wxString& wxstr) 
{
	return static_cast<const char*>(wxstr.c_str());
}




}


