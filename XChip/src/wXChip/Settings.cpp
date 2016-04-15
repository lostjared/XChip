#include <wXChip/Settings.h>


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX) {
    
    SetMinSize(GetSize());
    SetMaxSize(GetSize());
}

wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
wxEND_EVENT_TABLE()