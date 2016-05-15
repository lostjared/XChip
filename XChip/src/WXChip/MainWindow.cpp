#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#ifdef _WIN32
#include <WXChip/dirent.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <dirent.h>
#endif


#include <iostream>
#include <fstream>
#include <regex>
#include <stdexcept>

#include <XChip/Utility/Memory.h>
#include <WXChip/Main.h>
#include <WXChip/SaveList.h>
#include <WXChip/MainWindow.h>


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_EXIT, MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MENU(MainWindow::ID_LoadRom, MainWindow::OnLoadRom)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_LOADROMDIR, MainWindow::OnLoadRomDir)
EVT_BUTTON(ID_EMUSET, MainWindow::OnLoadSettings)
wxEND_EVENT_TABLE()


MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, 0, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	using xchip::utility::make_unique;

	std::cout << "Creating MainWindow..." << std::endl;

	if(!ComputeEmuAppPath())
		throw std::runtime_error("could not find EmuApp executable!");


	CreateControls();

	auto menuFile = make_unique<wxMenu>();

	menuFile->Append(ID_LoadRom, "&LoadRom...\tCtrl-L", "Load a game rom");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuHelp = make_unique<wxMenu>();
	menuHelp->Append(wxID_ABOUT);


	auto menuBar = make_unique<wxMenuBar>();

	if (!menuBar->Append(menuFile.get(), "&File") || !menuBar->Append(menuHelp.release(), "&Help"))
		throw std::runtime_error("could not append a menu into wxMenuBar");

	menuFile.release();
	SetMenuBar(menuBar.release());

	CreateStatusBar();
	SetStatusText("Welcome to WXChip");
}


MainWindow::~MainWindow()
{
	StopEmulator();
	Destroy();
	std::cout << "Destroying MainWindow..." << std::endl;
}





void MainWindow::OnExit(wxCommandEvent&)
{
	Close(true);
}



void MainWindow::OnAbout(wxCommandEvent&)
{
	wxMessageBox("WXChip - Chip8 Emulator",
		"About WXChip", wxOK | wxICON_INFORMATION);
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
	auto m_lbox = static_cast<wxListBox*>(event.GetEventObject());
	int item = m_lbox->HitTest(event.GetPosition());

	if (item != wxNOT_FOUND)
	{
		wxString str = m_lbox->GetString(item);
		_romPath = _settingsWin->GetRomPath();
#ifdef _WIN32
		_romPath += '\\';
#elif defined(__APPLE__) || defined(__linux__)
		_romPath += '/';
#endif
		_romPath += str.c_str();
		std::cout << "Start Rom At Path: " << _romPath << "\n";

		StartEmulator();
		
	}
}


void MainWindow::OnLoadSettings(wxCommandEvent&)
{
	_settingsWin->Show(true);
}



void MainWindow::OnStartRom(wxCommandEvent&)
{
	std::cout << "Starting Rom...\n";
	StartEmulator();
}


void MainWindow::OnLoadRomDir(wxCommandEvent& event)
{
	wxDirDialog dlg(NULL, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;


	wxString value = dlg.GetPath();
	std::string fps = _settingsWin->FPS();
	std::string cpu = _settingsWin->CPUFreq();
	LoadList(std::string(value.c_str()), fps, cpu);
}



void MainWindow::OnLoadRom(wxCommandEvent&)
{
	StopEmulator();

	wxFileDialog openDialog(this, "", "", "", "All Files (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	// the user didn't select any file ?
	if (openDialog.ShowModal() == wxID_CANCEL)
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
	if (_process.IsRunning())
	{
		_process.Terminate();
	}
}




void MainWindow::CreateControls()
{
	using xchip::utility::make_unique;
	wxArrayString strings;
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_text = make_unique<wxStaticText>(_panel.get(), ID_TEXT, _T("Chip8 Roms"), wxPoint(10, 10), wxSize(100, 25));
	_listBox = make_unique<wxListBox>(_panel.get(), ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), strings, wxLB_SINGLE);
	_listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);
	_startRom = make_unique<wxButton>(_panel.get(), ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100, 25));
	_settings = make_unique<wxButton>(_panel.get(), ID_LOADROMDIR, _T("Load Roms"), wxPoint(120, 400), wxSize(100, 25));
	_emulatorSettings = make_unique<wxButton>(_panel.get(), ID_EMUSET, _T("Settings"), wxPoint(230, 400), wxSize(100, 25));
	_settingsWin = make_unique<SettingsWindow>("WXChip - Settings", wxPoint(150, 150), wxSize(430, 220));

}








void MainWindow::LoadList(const std::string &text, const std::string &fps, std::string &cpu_freq)
{

	saveDirectory(text, fps, cpu_freq);

	if (text == "nopath") return;

	wxArrayString strings;

	_listBox->Clear();

	DIR *dir = opendir(text.c_str());

	if (dir == NULL)
	{
		std::cerr << "Error could not open directory.\n";
		return;
	}

	dirent *e;

	while ((e = readdir(dir)))
	{
		if (e->d_type == DT_REG)
		{
			std::string text = e->d_name;
			std::regex exp1("ch8$", std::regex_constants::icase);
			std::regex exp2("([0-9a-zA-Z_\\ ]+)", std::regex_constants::icase);
			bool isTag = std::regex_search(text, exp1);
			bool isTag2 = std::regex_match(text, exp2);
			if (isTag || isTag2)
			{
				wxString w(e->d_name);
				strings.Add(w);
			}
		}
	}

	closedir(dir);


	if (!strings.IsEmpty())
	{
		_listBox->InsertItems(strings, 0);
		_romPath = text;
		_settingsWin->SetRomPath(text, fps, cpu_freq);
	}
	else
	{
		_settingsWin->SetRomPath("", fps, cpu_freq);

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


	if (isFullPath)
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


	if (!std::ifstream(_emuApp).good())
	{
		std::cerr << "EmuApp executable not found!" << std::endl;
		return false;
	}

	_emuApp.insert(0, "\"");
	_emuApp += "\" ";

	std::cout << "_emuApp after compute: " << _emuApp << std::endl;

	return true;
}


