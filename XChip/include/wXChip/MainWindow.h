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

#include <wXChip/SaveList.h>



class wXChip: public wxApp
{
public:
	virtual bool OnInit();
};




class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	void LoadList(const std::string &text);

    
private:
	void OnChip(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnLDown(wxMouseEvent &event);
	void OnStartRom(wxCommandEvent &event);
	void LoadSettings(wxCommandEvent &event);
	void LaunchRom();
	void OnMouseOver(wxMouseEvent &event);
	void OnSize(wxSizeEvent &event);

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;

	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::string _filePath;

	wxDECLARE_EVENT_TABLE();
    
};
enum { ID_Chip = 1, ID_LISTBOX = 2, ID_STARTROM = 3, ID_LOADROM = 4, ID_TEXT = 5, ID_EMUSET };




wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(ID_Chip,   MainWindow::OnChip)
EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
EVT_MOTION(MainWindow::OnMouseOver)
EVT_BUTTON(ID_STARTROM, MainWindow::OnStartRom)
EVT_BUTTON(ID_LOADROM, MainWindow::OnChip)
EVT_BUTTON(ID_EMUSET, MainWindow::LoadSettings)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(wXChip);

//xBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
//wxEND_EVENT_TABLE()