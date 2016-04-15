/* wXChip
   (c) 2016
*/

#include <sstream>
#include <XChip/Utility/Memory.h>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/listbox.h>

#if defined(__APPLE__) || defined(__linux__)
#include <dirent.h>
#elif defined(_WIN32)
#include <wXChip/dirent.h>
#endif

#include <wXChip/savelist.h>


class MainWindow;


class wXChip: public wxApp
{
public:
    virtual bool OnInit();

};

class MainWindow: public wxFrame
{
public:
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
    std::unique_ptr<wxListBox> _listBox;
    std::unique_ptr<wxButton> _startRom;
    std::unique_ptr<wxButton> _settings;
    std::unique_ptr<wxButton> _emulatorSettings;
    std::unique_ptr<wxStaticText> _text;
    void LoadList(const std::string &text);
    std::string _filePath;
    
private:
    void OnChip(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnLDown(wxMouseEvent &event);
    void OnStartRom(wxCommandEvent &event);
    void LoadSettings(wxCommandEvent &event);
    void LaunchRom();
    wxDECLARE_EVENT_TABLE();
    
};
enum { ID_Chip = 1, ID_LISTBOX = 2, ID_STARTROM = 3, ID_SETTINGS = 4, ID_TEXT = 5, ID_EMUSET};




wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_Chip,   MainWindow::OnChip)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_SETTINGS, MainWindow::OnChip)
EVT_BUTTON(ID_EMUSET, MainWindow::LoadSettings)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(wXChip);

//xBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
//wxEND_EVENT_TABLE()



bool wXChip::OnInit()
{
    using xchip::utility::make_unique;    
    
    const std::string file = getDirectory();
    
    auto _frame = new MainWindow( "wXChip ", wxPoint(50, 50), wxSize(640, 480) );
    _frame->Show( true );
    
    if(file != "nolist") 
        _frame->LoadList(file);
    
    return true;
}



MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
   using xchip::utility::make_unique;


    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Chip, "&Load Roms...\tCtrl-L",
                     "Load Roms");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wXChip" );
    
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxArrayString strings;
    
//    wxStaticText *text = new wxStaticText(panel, ID_TEXT, _T("Chip8 Roms"), wxPoint(10,10), wxSize(100,25));
    
    _text = make_unique<wxStaticText>(panel, ID_TEXT, _T("Chip8 Roms"), wxPoint(10,10), wxSize(100,25));
    
    _listBox = make_unique<wxListBox>(panel, ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), strings, wxLB_SINGLE);
    _listBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);
    
    _startRom = make_unique<wxButton>(panel, ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100,25));
    _settings = make_unique<wxButton>(panel, ID_SETTINGS, _T("Load Roms"), wxPoint(120, 400), wxSize(100,25));
    _emulatorSettings = make_unique<wxButton>(panel, ID_EMUSET, _T("Settings"), wxPoint(230, 400), wxSize(100,25));
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
    auto m_lbox = static_cast<wxListBox*>(event.GetEventObject());

    // Get the item index
    int item = m_lbox->HitTest(event.GetPosition());
    
    if ( item != wxNOT_FOUND ) {
        wxString str = m_lbox->GetString(item);
        std::ostringstream stream;
        stream << _filePath << "/" << str.c_str();
        std::string fullname = stream.str();
        std::cout << "Start Rom At Path: " << fullname << "\n";
        wxString fname(fullname.c_str());
        wxLogMessage(fname);
        
    }
  
//    else
//        wxLogMessage(_T("Listbox right clicked but no item clicked upon"));
}


void MainWindow::LoadSettings(wxCommandEvent &event) {
    
}

void MainWindow::OnStartRom(wxCommandEvent &event) {
    std::cout << "Starting Rom...\n";
    LaunchRom();
    // start application
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close( true );
}

void MainWindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "wXChip - xChip8 Emulator",
                  "About wXChip", wxOK | wxICON_INFORMATION );
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
}



void MainWindow::LaunchRom() 
{
    // Get the item index
    int item = _listBox->GetSelection();
    
    if (item != wxNOT_FOUND ) {
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
