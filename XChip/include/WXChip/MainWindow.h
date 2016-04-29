#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_


#include <wx/frame.h>




class MainWindow final : public wxFrame
{
public:
	enum { ID_LOADROM };

	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();


private:
	void OnExit(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);
	void OnKeyDown(wxKeyEvent& ev);

	wxDECLARE_EVENT_TABLE();
};





#endif
