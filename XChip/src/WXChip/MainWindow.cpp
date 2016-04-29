#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <XChip/Utility/Memory.h>
#include <WXChip/MainWindow.h>


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(MainWindow::ID_LOADROM, MainWindow::OnLoadRom)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	using xchip::utility::make_unique;
	std::cout << "Creating MainWindow..." << std::endl;

	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_LOADROM, "&LoadRom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);	

	auto menuBar = make_unique<wxMenuBar>();

	if (!menuBar->Append(menuFile.get(), "&File"))
		throw std::exception("could not append a menu into wxMenuBar");

	
	menuFile.release();
	SetMenuBar(menuBar.release());
}


MainWindow::~MainWindow()
{
	std::cout << "Destroying MainWindow..." << std::endl;
	this->Destroy();
}


void MainWindow::OnExit(wxCommandEvent&)
{
	Close( true );	
}


void MainWindow::OnLoadRom(wxCommandEvent&)
{

	wxFileDialog openDialog(this, "","","", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// the user didn't select any file ?
	if(openDialog.ShowModal() == wxID_CANCEL)
		return;

	
	// the user selected some file:
	std::cout << "loading rom: " << openDialog.GetPath().c_str() << std::endl;
}





