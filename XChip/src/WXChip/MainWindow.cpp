#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <stdexcept>

#include <XChip/Utility/Memory.h>
#include <WXChip/Main.h>
#include <WXChip/MainWindow.h>


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(MainWindow::ID_LoadRom, MainWindow::OnLoadRom)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	using xchip::utility::make_unique;

	std::cout << "Creating MainWindow..." << std::endl;

	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_LoadRom, "&LoadRom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);	

	auto menuBar = make_unique<wxMenuBar>();

	if (!menuBar->Append(menuFile.get(), "&File"))
		throw std::runtime_error("could not append a menu into wxMenuBar");

	menuFile.release();
	SetMenuBar(menuBar.release());



	// get _emuApp path:
	char path[256];
	char *cwd;

#ifdef _WIN32
	cwd = _getcwd(path, 255);
#elif defined(__APPLE__) || defined(__linux__)
	cwd = getcwd(path, 255);
#endif

	_emuApp += "\"";
	_emuApp += cwd;


#ifdef _WIN32 

	std::string wxchipPath = static_cast<const char*>(wxTheApp->argv[0].c_str());
	const auto firstSep = wxchipPath.find_first_of('\\');
	const auto isFullPath = firstSep != std::string::npos && wxchipPath[firstSep - 1] == ':';

	// isn't the full path like C:\ ... ?
	// then we need to complete the path
	if (!isFullPath)
	{
		const auto lastSep = wxchipPath.find_last_of('\\');
		if (lastSep != std::string::npos)
		{
			const auto wxchipDir = wxchipPath.substr(0, lastSep);

			if (wxchipDir != cwd)
			{
				_emuApp += "\\";
				_emuApp += wxchipDir;
			}
		}
	}


#elif defined(__APPLE__) || defined(__linux__)

	std::string wxchipPath = static_cast<const char*>(wxTheApp->argv[0].c_str());
	const auto lastSep = wxchipPath.find_last_of('/');

	if (lastSep > 1)
	{
		const auto wxchipDir = wxchipPath.substr(0, lastSep);
		if (wxchipDir != cwd)
		{
			_emuApp += "/";
			_emuApp += wxchipDir;
		}
	}


#endif

	_emuApp += defaultEmuAppPath;
	_emuApp += "\"";
}


MainWindow::~MainWindow()
{
	StopEmulator();
	Destroy();
	std::cout << "Destroying MainWindow..." << std::endl;
}


void MainWindow::OnExit(wxCommandEvent&)
{
	Close( true );
}



void MainWindow::OnLoadRom(wxCommandEvent&)
{
	StopEmulator();

	wxFileDialog openDialog(this, "","","", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	// the user didn't select any file ?
	if(openDialog.ShowModal() == wxID_CANCEL)
		return;


	_romPath = openDialog.GetPath().c_str();
	
	std::cout << "Selected File: " << _romPath << std::endl;

	StartEmulator();
}


void MainWindow::StartEmulator()
{
	StopEmulator();
	_process.Run(_emuApp + " \"" + _romPath + "\"");
}


void MainWindow::StopEmulator()
{
	if(_process.IsRunning())
	{
		_process.Terminate();
	}
}
