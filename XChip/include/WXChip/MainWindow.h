#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_
#include <string>
#include <wx/frame.h>
#include <XChip/Utility/Process.h>

class MainWindow final : public wxFrame
{
	friend class WXChip;
#ifdef _WIN32
	static constexpr const char* const defaultEmuAppPath = "\\bin\\EmuApp";
#elif defined(__APPLE__) || defined(__linux__)
	static constexpr const char* const defaultEmuAppPath = "/bin/EmuApp";
#endif

public:
	enum { ID_LoadRom };
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();


private:
	void OnExit(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);
	void StartEmulator();
	void StopEmulator();

	std::string _emuApp;
	std::string _romPath;
	xchip::utility::Process _process;
	wxDECLARE_EVENT_TABLE();
};





#endif
