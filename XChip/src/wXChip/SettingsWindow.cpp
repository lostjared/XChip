#include <wXChip/SettingsWindow.h>
#include<wx/valnum.h>


enum { ID_TEXT1 = 1, ID_TEXTCTRL1, ID_TEXT2, ID_TEXTCTRL2, ID_TEXT3, ID_TEXTCTRL3 , ID_BTN_OK, ID_BTN_CANCEL };



wxBEGIN_EVENT_TABLE(SettingsWindow, wxFrame)
EVT_CLOSE(SettingsWindow::OnCloseWindow)
EVT_BUTTON(ID_BTN_OK, SettingsWindow::OnOkay)
EVT_BUTTON(ID_BTN_CANCEL, SettingsWindow::OnCancel)
wxEND_EVENT_TABLE()


SettingsWindow::SettingsWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{

	CreateControls();
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void SettingsWindow::setRomPath(const std::string &text)
{
	_romPath->Clear();
	*_romPath.get() << text.c_str();
	
}

void SettingsWindow::CreateControls()
{
	using xchip::utility::make_unique;
	_panel = make_unique<wxPanel>(this, wxID_ANY);
	_romText = make_unique<wxStaticText>(_panel.get(), ID_TEXT1,_T("Rom Path: "), wxPoint(10,15), wxSize(150,25));
	_romPath = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, "", wxPoint(100,10), wxSize(320,20),wxTE_READONLY);
	
	wxFloatingPointValidator<float> _val(2,&_fpsValue,wxNUM_VAL_ZERO_AS_BLANK);
	_val.SetRange(1.0,30.0);
	
	_fpsText = make_unique<wxStaticText>(_panel.get(), ID_TEXT2,_T("FPS: "), wxPoint(10,40), wxSize(150,25));
	_fps = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL1, _T("30"), wxPoint(100,40), wxSize(100,20),0,_val);
	_cpuText = make_unique<wxStaticText>(_panel.get(), ID_TEXT3,_T("CPU Freq: "), wxPoint(220,40), wxSize(150,25));
	_cpu = make_unique<wxTextCtrl>(_panel.get(), ID_TEXTCTRL3, _T(""), wxPoint(320,40), wxSize(100,20));
	_buttonOk = make_unique<wxButton>(_panel.get(), ID_BTN_OK, _T("Ok"), wxPoint(10, 150), wxSize(100,25));
	_buttonCancel = make_unique<wxButton>(_panel.get(), ID_BTN_CANCEL, _T("Cancel"), wxPoint(120, 150), wxSize(100,25));
}

void SettingsWindow::SaveSettings()
{
	
}

void SettingsWindow::OnCloseWindow(wxCloseEvent &event)
{
	Show(false);
	event.Veto();
}


void SettingsWindow::OnCancel(wxCommandEvent &event)
{
	Show(false);
}

void SettingsWindow::OnOkay(wxCommandEvent &event)
{
	// save settings
	SaveSettings();
	Show(false);
}




