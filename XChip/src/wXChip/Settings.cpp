#include <wXChip/Settings.h>


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX) {
    
        SetMinSize(GetSize());
        SetMaxSize(GetSize());
}

void SettingsWindow::OnCloseWindow(wxCloseEvent &event) {
	Show(false);
	event.Veto();
}

wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
wxEND_EVENT_TABLE()


