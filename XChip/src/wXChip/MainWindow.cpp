/* wXChip
   (c) 2016
*/

#include <sstream>
#include <wXChip/MainWindow.h>

#if defined(__APPLE__) || defined(__linux__)
#include <dirent.h>
#elif defined(_WIN32)
#include <wXChip/dirent.h>
#endif

#include <wXChip/SaveList.h>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_Chip,   MainWindow::OnChip)
EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
EVT_CLOSE(MainWindow::OnWindowClose)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MOTION(MainWindow::OnMouseOver)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_LOADROM, MainWindow::OnChip)
EVT_BUTTON(ID_EMUSET, MainWindow::LoadSettings)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(wXChip);

bool wXChip::OnInit()
{
	using xchip::utility::make_unique;
    
	const std::string file = getDirectory();
	auto frame = make_unique<MainWindow>( "wXChip ", wxPoint(50, 50), wxSize(800, 600) );
	frame->Show( true );
   
	if(file != "nolist")
		frame->LoadList(file);
	
	frame.release();    
	return true;
}



MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	using xchip::utility::make_unique;


	auto menuFile = make_unique<wxMenu>();
	menuFile->Append(ID_Chip, "&Load Roms...\tCtrl-L", 
                         "Load Roms");
    
	menuFile->Append(ID_EMUSET, "&Settings\tCtrl-S", "Settings");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuHelp = make_unique<wxMenu>();
	menuHelp->Append(wxID_ABOUT);

	auto menuBar = make_unique<wxMenuBar>();
	menuBar->Append( menuFile.release(), "&File" );
	menuBar->Append( menuHelp.release(), "&Help" );
	SetMenuBar( menuBar.release() );

	CreateStatusBar();
	SetStatusText( "Welcome to wXChip" );


	wxArrayString strings;

	_panel = make_unique<wxPanel>(this, wxID_ANY);

	_text = make_unique<wxStaticText>(_panel.get(), ID_TEXT, _T("Chip8 Roms"), wxPoint(10,10), wxSize(100,25));

	_listBox = make_unique<wxListBox>(_panel.get(), ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), strings, wxLB_SINGLE);
	_listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);

	_startRom = make_unique<wxButton>(_panel.get(), ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100,25));
	_settings = make_unique<wxButton>(_panel.get(), ID_Chip, _T("Load Roms"), wxPoint(120, 400), wxSize(100,25));
	_emulatorSettings = make_unique<wxButton>(_panel.get(), ID_EMUSET, _T("Settings"), wxPoint(230, 400), wxSize(100,25));

	_settingsWin = make_unique<SettingsWindow>("wXChip - Settings", wxPoint(150, 150), wxSize(640, 480));
	
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
	auto m_lbox = static_cast<wxListBox*>(event.GetEventObject());

	// Get the item index
	int item = m_lbox->HitTest(event.GetPosition());
    
	if ( item != wxNOT_FOUND ) 
	{
		wxString str = m_lbox->GetString(item);
		std::ostringstream stream;
		stream << _filePath << "/" << str.c_str();
		std::string fullname = stream.str();
		std::cout << "Start Rom At Path: " << fullname << "\n";
		wxString fname(fullname.c_str());
		wxLogMessage(fname);

	}
}


void MainWindow::OnStartRom(wxCommandEvent &event) {
	std::cout << "Starting Rom...\n";
	LaunchRom();
    // start application
}

void MainWindow::LoadSettings(wxCommandEvent &event) {
	_settingsWin->Show(true);
}

void MainWindow::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "wXChip - Chip8 Emulator",
                     "About wXChip", wxOK | wxICON_INFORMATION );
}

void MainWindow::OnMouseOver(wxMouseEvent &event) {
    
}

void MainWindow::OnSize(wxSizeEvent& event) {
    
}


void MainWindow::OnWindowClose(wxCloseEvent &event) {
	Destroy();
	// Cleanup here
}



void MainWindow::LoadList(const std::string &text) 
{

	saveDirectory(text);

	wxArrayString strings;

	_listBox->Clear();

	DIR *dir = opendir(text.c_str());

	if(dir == NULL) {
		std::cerr << "Error could not open directory.\n";
		return;
	}
    
	dirent *e;

	while((e = readdir(dir))) 
	{
		if(e->d_type == 0x8) {
			wxString w(e->d_name);
			strings.Add(w);
		}
	}
    
	closedir(dir);
	_listBox->InsertItems(strings, 0);
	_filePath = text;
	_settingsWin->setRomPath(text);
}



void MainWindow::LaunchRom() 
{
	// Get the item index
	int item = _listBox->GetSelection();
    
	if (item != wxNOT_FOUND ) 
	{
		const wxString str = _listBox->GetString(item);
		std::ostringstream stream;
		stream << _filePath << "/" << str.c_str();
		const std::string fullname = stream.str();
		std::cout << "Start Rom At Path: " << fullname << "\n";
		wxString fname(fullname.c_str());
		wxLogMessage(fname);
	}
}



void MainWindow::OnChip(wxCommandEvent& event)
{
	wxDirDialog dlg(NULL, "Choose input directory", "",
                        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...


	wxString value = dlg.GetPath();
	LoadList(std::string(value.c_str()));
}
