#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_

#include <XChip/Utility/Memory.h>
#include <wXChip/SettingsWindow.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Core/Emulator.h>
#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>
#include<atomic>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/listbox.h>


class MainWindow;

class wXChip: public wxApp
{
	bool render_loop_on;
	virtual bool OnInit();
	void onIdle(wxIdleEvent& evt);
	MainWindow *main;
public:
	void activateRenderLoop(bool on);
};

class RunEmulator {
public:
	xchip::Emulator emu;
	bool load(const std::string &text);
	void init();
	void stop();
	void update();
	
	RunEmulator() : closing(false) {
	
	}
	
	~RunEmulator() {
		stop();
	}
	
	bool closing;
private:
	

};

class MainWindow: public wxFrame
{
public:
	MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	void LoadList(const std::string &text, const std::string &fps, std::string &cpu_freq);
	void CreateControls();
	void UpdateEmulator();
	bool running = false;
private:
	wxTimer _timer;
	void OnChip(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnLDown(wxMouseEvent &event);
	void OnMouseOver(wxMouseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnWindowClose(wxCloseEvent &event);
	void OnTimer(wxTimerEvent& event);
	void OnStartRom(wxCommandEvent &event);
	void LoadSettings(wxCommandEvent &event);
	void LaunchRom();
	void StartProgram(const std::string &rom);

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;

	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::unique_ptr<SettingsWindow> _settingsWin;
	
	std::string _filePath;
	
	
	RunEmulator *emulator;

	wxDECLARE_EVENT_TABLE();
	std::string current_rom;
	
};








#endif