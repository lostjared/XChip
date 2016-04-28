#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
	
	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_LOADROM, "&LoadRom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);	

	auto menuBar = make_unique<wxMenuBar>();	
	menuBar->Append(menuFile.release(), "&File");


	SetMenuBar(menuBar.release());
}


MainWindow::~MainWindow()
{
	this->Destroy();
}


void MainWindow::OnExit(wxCommandEvent&)
{
	Close( true );	
}


void MainWindow::OnLoadRom(wxCommandEvent&)
{
	std::cout << "loading some rom" << std::endl;
}





