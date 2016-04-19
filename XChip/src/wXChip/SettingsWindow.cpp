#include <wXChip/SettingsWindow.h>

enum { ID_TEXT1 = 1, ID_TEXTCTRL1 };


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX) {

	CreateControls();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void SettingsWindow::setRomPath(const std::string &text) {
	std::ostream stream(_romPath.get());
	stream << text;
	stream.flush();
}

void SettingsWindow::CreateControls() noexcept {

	using xchip::utility::make_unique;
	
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_romText = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Rom Path: "), wxPoint(10,10), wxSize(150,25));
	_romPath = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", wxPoint(100,10), wxSize(350,25),wxTE_READONLY);
	
}

void SettingsWindow::OnCloseWindow(wxCloseEvent &event) {
	Show(false);
	event.Veto();
}


wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
wxEND_EVENT_TABLE()


