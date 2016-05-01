#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <stdexcept>

#include <XChip/Utility/Memory.h>
#include <WXChip/MainWindow.h>



extern int start_emulator(void*);



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
	using xchip::utility::make_unique;

	wxFileDialog openDialog(this, "","","", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// the user didn't select any file ?
	if(openDialog.ShowModal() == wxID_CANCEL)
		return;


	std::string filePath { openDialog.GetPath().c_str() };

	
	std::cout << "Loading : " << filePath << std::endl;

	StartEmulator(filePath);
	
}


void MainWindow::StartEmulator(std::string& arg)
{
	StopEmulator();
	_process.Run(start_emulator, (void*)arg.c_str());
	_emuProcOn = true;
}


void MainWindow::StopEmulator()
{
	if(_emuProcOn)
	{
		_process.Stop();
		_emuProcOn = false;
	}
}
