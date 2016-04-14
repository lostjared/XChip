/* wXChip
   (c) 2016
*/

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include<wx/listbox.h>

#if defined(__APPLE__) || defined(__linux__)
#include<dirent.h>
#elif defined(_WIN32)
#include "dirent.h"
#endif


class wXChip: public wxApp
{
public:
    virtual bool OnInit();
};

class MainWindow: public wxFrame
{
public:
    MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
    
    wxListBox *ListBox;
    wxButton *startRom;
    wxButton *settings;
    
private:
    void OnChip(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnLDown(wxMouseEvent &event);
    void OnStartRom(wxCommandEvent &event);
    
    wxDECLARE_EVENT_TABLE();
    
};
enum { ID_Chip = 1, ID_LISTBOX = 2, ID_STARTROM = 3, ID_SETTINGS = 4, ID_TEXT = 5 };

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_Chip,   MainWindow::OnChip)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_SETTINGS, MainWindow::OnChip)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(wXChip);

bool wXChip::OnInit()
{
    MainWindow *frame = new MainWindow( "wXChip ", wxPoint(50, 50), wxSize(640, 480) );
    frame->Show( true );
    return true;
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
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
    
    wxStaticText *text = new wxStaticText(panel, ID_TEXT, _T("Chip8 Roms"), wxPoint(10,10), wxSize(100,25));
    
    ListBox = new wxListBox(panel, ID_LISTBOX, wxPoint(10, 35), wxSize(620, 360), strings, wxLB_SINGLE);
    ListBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainWindow::OnLDown), NULL, this);
    startRom = new wxButton(panel, ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100,25));
    settings = new wxButton(panel, ID_SETTINGS, _T("Load Roms"), wxPoint(120, 400), wxSize(100,25));
}


void MainWindow::OnLDown(wxMouseEvent& event)
{
    wxListBox* m_lbox = dynamic_cast<wxListBox*>(event.GetEventObject());
    
    // Get the item index
    int item = m_lbox->HitTest(event.GetPosition());
    
    if ( item != wxNOT_FOUND ) {
        wxLogMessage(_T("Listbox item %d clicked"), item);
        wxString str = m_lbox->GetString(item);
        std::cout << "Start Rom: " << str.c_str() << "\n";
    }
    else
        wxLogMessage(_T("Listbox right clicked but no item clicked upon"));
}



void MainWindow::OnStartRom(wxCommandEvent &event) {
    std::cout << "Starting Rom...\n";
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

void MainWindow::OnChip(wxCommandEvent& event)
{
    wxDirDialog dlg(NULL, "Choose input directory", "",
                    wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    if (dlg.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
 
    wxArrayString strings;
    
    
//#if defined(__APPLE__) || defined(__linux__)
    
    DIR *dir = opendir(dlg.GetPath().c_str());
    
    if(dir == NULL) {
        std::cerr << "Error could not open directory.\n";
        return;
    }
    
    dirent *e;
    
    while((e = readdir(dir))) {
        if(e->d_type == 0x8) {
            wxString w(e->d_name);
            strings.Add(w);
        }
    }
    
    closedir(dir);
    ListBox->InsertItems(strings, 0);
    
//#else // windows
    
//#endif
    
}
