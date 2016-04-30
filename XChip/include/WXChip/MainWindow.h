#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_


#include <wx/frame.h>
#include <XChip/Core/Emulator.h>



class MainWindow final : public wxFrame
{
	friend class WXChip;
public:
	enum { ID_LoadRom };

	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();

private:
	void OnExit(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);
	void RunEmulator();
	void StopEmulator();
	void EmulatorLoop(wxIdleEvent&);

	bool _emuLoopOn;
	xchip::Emulator _emulator;
	wxDECLARE_EVENT_TABLE();
};





#endif
