#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <fstream>
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

	if(!ComputeEmuAppPath())
		throw std::runtime_error("could not find EmuApp executable!");

	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_LoadRom, "&LoadRom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);	

	auto menuBar = make_unique<wxMenuBar>();

	if (!menuBar->Append(menuFile.get(), "&File"))
		throw std::runtime_error("could not append a menu into wxMenuBar");

	menuFile.release();
	SetMenuBar(menuBar.release());
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



bool MainWindow::ComputeEmuAppPath()
{

#ifdef _WIN32

	constexpr const char dirSlash = '\\';
	constexpr const char* const finalEmuAppPath = "\\bin\\EmuApp.exe";

#elif defined(__APPLE__) || defined(__linux__)

	constexpr const char dirSlash = '/';
	constexpr const char* const finalEmuAppPath = "/bin/EmuApp";
#endif


	const std::string wxchipPath = static_cast<const char*>(wxTheApp->argv[0].c_str());
	const auto firstSlash = wxchipPath.find_first_of(dirSlash);
	const auto lastSlash = wxchipPath.find_last_of(dirSlash);

#ifdef _WIN32

	const auto isFullPath = firstSlash != std::string::npos && wxchipPath[firstSlash - 1] == ':'; // like C:\WXChip.exe instead of WXChip.exe

#elif defined(__APPLE__) || defined(__linux__)

	const auto isFullPath = firstSlash == 0; // like /home/WXChip instead of ./WXChip
#endif

	std::cout << "wxchipPath: " << wxchipPath << std::endl;
	std::cout << "firstSlash: " << firstSlash << std::endl;
	std::cout << "isFullPath: " << isFullPath << std::endl;


	if(isFullPath)
	{
		_emuApp += wxchipPath.substr(0, lastSlash);
	}

	else
	{
		char cwd[256];
#ifdef _WIN32
		_getcwd(cwd, 255);
#elif defined(__APPLE__) || defined(__linux__)
		getcwd(cwd, 255);
#endif
		_emuApp += cwd;
		_emuApp += dirSlash;
		_emuApp += wxchipPath.substr(0, lastSlash);
	}
	

	_emuApp += finalEmuAppPath;


	if(!std::ifstream(_emuApp).good())
	{
		std::cerr << "EmuApp executable not found!" << std::endl;
		return false;
	}

	_emuApp.insert(0, "\"");
	_emuApp += "\" ";

	std::cout << "_emuApp after compute: " << _emuApp << std::endl;

	return true;
}
