#include <wXChip/SettingsWindow.h>

enum { ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3 , ID_BTN_OK, ID_BTN_CANCEL };


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{

	CreateControls();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void SettingsWindow::setRomPath(const std::string &text)
{
	std::ostream stream(_romPath.get());
	stream << text;
	stream.flush();
}

void SettingsWindow::CreateControls()
{

	using xchip::utility::make_unique;
	
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_romText = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Rom Path: "), wxPoint(10,15), wxSize(150,25));
	_romPath = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", wxPoint(100,10), wxSize(350,20),wxTE_READONLY);
	
	_fpsText = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), wxPoint(10,40), wxSize(150,25));

	_fps = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", wxPoint(100,40), wxSize(100,20));

	_cpuText = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Freq: "), wxPoint(220,40), wxSize(150,25));
	
	_cpu = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, "", wxPoint(320,40), wxSize(100,20));
	
	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BTN_OK, _T("Ok"), wxPoint(10, 150), wxSize(100,25));
	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BTN_CANCEL, _T("Cancel"), wxPoint(120, 150), wxSize(100,25));

	
}

void SettingsWindow::OnCloseWindow(wxCloseEvent &event)
{
	Show(false);
	event.Veto();
}


wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
wxEND_EVENT_TABLE()


