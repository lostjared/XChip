#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_
#include <XChip/Core/Emulator.h>
#include <XChip/Utility/Memory.h>
#include <wXChip/SettingsWindow.h>






class wXChip: public wxApp
{
public:
	virtual bool OnInit();
};


class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	void LoadList(const std::string &text, const std::string &fps, std::string &cpu_freq);
	void CreateControls();
	void CreateEmulator();
	bool running, closing;
private:
	
	void OnChip(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnLDown(wxMouseEvent& event);
	void OnMouseOver(wxMouseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnWindowClose(wxCloseEvent& event);
	void OnStartRom(wxCommandEvent& event);
	void OnKeyUp(wxKeyEvent &key);
	void OnKeyDown(wxKeyEvent &key);
	void LoadSettings(wxCommandEvent& event);
	void LaunchRom();
	void StartProgram(const std::string& rom);
	void OnIdle(wxIdleEvent& event);
	void StartEmulatorLoop();
	void StopEmulatorLoop();


	bool _emuLoopOn = false;
	std::unique_ptr<xchip::Emulator> _emu;
	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;
	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::unique_ptr<SettingsWindow> _settingsWin;
	std::string _filePath;
	
	wxDECLARE_EVENT_TABLE();
    
};

#endif
