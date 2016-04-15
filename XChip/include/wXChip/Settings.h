#ifndef __SETTINGS__H__
#define __SETTINGS__H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class SettingsWindow: public wxFrame {
public:
	SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize);
   
private:
	wxDECLARE_EVENT_TABLE();
};



#endif
