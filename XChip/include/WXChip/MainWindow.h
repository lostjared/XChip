#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_
#include <string>

#include <wx/frame.h>
#include <XChip/Utility/Process.h>
#include <WXChip/SettingsWindow.h>

class MainWindow final : public wxFrame
{
	friend class WXChip;
public:
	enum { ID_LoadRom, ID_TEXT, ID_LISTBOX, ID_STARTROM, ID_LOADROM, ID_EMUSET, ID_LOADROMDIR };
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainWindow();


private:
	void OnExit(wxCommandEvent& ev);
	void OnAbout(wxCommandEvent& ev);
	void OnLDown(wxMouseEvent& ev);
	void OnStartRom(wxCommandEvent& ev);
	void OnLoadSettings(wxCommandEvent& ev);
	void OnLoadRomDir(wxCommandEvent& ev);
	void OnLoadRom(wxCommandEvent& ev);
	void StartEmulator();
	void StopEmulator();
	
	void CreateControls();
	void LoadList(const std::string &text, const std::string &fps, std::string &cpu_freq);
	bool ComputeEmuAppPath();

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;
	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::unique_ptr<SettingsWindow> _settingsWin;


	std::string _emuApp;
	std::string _romPath;
	xchip::utility::Process _process;
	wxDECLARE_EVENT_TABLE();
};





#endif
