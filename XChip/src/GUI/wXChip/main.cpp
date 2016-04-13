/* wXChip
   (c) 2016
*/

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include<wx/listbox.h>


class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    
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
enum
{
    ID_Chip = 1, ID_LISTBOX = 2, ID_STARTROM = 3, ID_SETTINGS = 4
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Chip,   MyFrame::OnChip)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_BUTTON(ID_STARTROM, MyFrame::OnStartRom)
EVT_BUTTON(ID_SETTINGS, MyFrame::OnChip)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( "wXChip ", wxPoint(50, 50), wxSize(640, 480) );
    frame->Show( true );
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Chip, "&Settings...\tCtrl-S",
                     "Set settings");
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
    ListBox = new wxListBox(panel, ID_LISTBOX, wxPoint(10, 10), wxSize(620, 380), strings, wxLB_SINGLE);
    ListBox->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MyFrame::OnLDown), NULL, this);
    startRom = new wxButton(panel, ID_STARTROM, _T("Start Rom"), wxPoint(10, 400), wxSize(100,25));
    settings = new wxButton(panel, ID_SETTINGS, _T("Settings"), wxPoint(120, 400), wxSize(100,25));
}


void MyFrame::OnLDown(wxMouseEvent& event)
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
 /*
    wxArrayString strings;
    strings.Add(wxT("First string"));
    strings.Add(wxT("Second string"));
    strings.Add(wxT("Third string"));
    strings.Add(wxT("Fourth string"));
    strings.Add(wxT("Fifth string"));
    strings.Add(wxT("Sixth string"));
    
    m_lbox->InsertItems(strings, 0);
   */
}



void MyFrame::OnStartRom(wxCommandEvent &event) {
    std::cout << "Starting Rom...\n";
    // start application
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "wXChip - xChip8 Emulator",
                  "About wXChip", wxOK | wxICON_INFORMATION );
}

void MyFrame::OnChip(wxCommandEvent& event)
{
    // Load settings window for rom path
    wxLogMessage("wXChip Settings Here!");
}
