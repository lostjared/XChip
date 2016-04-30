#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <iostream>
#include <stdexcept>
#include <thread>

#include <XChip/Utility/Memory.h>
#include <XChip/Media/SDLMedia.h>
#include <WXChip/MainWindow.h>



wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(MainWindow::ID_LoadRom, MainWindow::OnLoadRom)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxWANTS_CHARS)
{
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
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
	Destroy();
	std::cout << "Destroying MainWindow..." << std::endl;
}


void MainWindow::OnExit(wxCommandEvent&)
{
	Close( true );
}



void MainWindow::OnLoadRom(wxCommandEvent&)
{
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;



	wxFileDialog openDialog(this, "","","", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// the user didn't select any file ?
	if(openDialog.ShowModal() == wxID_CANCEL)
		return;


	const auto fileName = openDialog.GetPath();
	// the user selected some file:
	std::cout << "loading rom: " << fileName.c_str()  << std::endl;


	if(_emulator.GetExitFlag())
	{
		auto render = make_unique<xchip::SdlRender>();
		auto input = make_unique<xchip::SdlInput>();
		auto sound = make_unique<xchip::SdlSound>();

		if(! _emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
			throw std::runtime_error("could not initialize xchip::Emulator");
	}

	
	if(!_emulator.LoadRom(static_cast<const char*>(fileName.c_str())))
	{

		std::cout << "Could not load the game!" << std::endl;
	}


	StartGame();
}

std::thread _emuTr;

void MainWindow::StartGame()
{

	this->Hide();
	
	const auto loop  = [this]()
	{	

		_emulator.CleanFlags();
		_emulator.GetRender()->ShowWindow();

		while( !_emulator.GetExitFlag() )
		{

			_emulator.UpdateSystems();
			_emulator.HaltForNextFlag();

			if(_emulator.GetInstrFlag())
				_emulator.ExecuteInstr();
			if(_emulator.GetDrawFlag())
				_emulator.Draw();
		

		}
		
		_emulator.GetRender()->HideWindow();
		
		this->Raise();
	};


	_emuTr = std::thread(loop);
	_emuTr.detach();
}
