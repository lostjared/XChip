#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_
#include<wXChip/RunEmulator.h>
#include <XChip/Utility/Memory.h>
#include <wXChip/SettingsWindow.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Timer.h>
#include <XChip/Core/Emulator.h>
#include <XChip/Media/SDLMedia/SdlRender.h>
#include <XChip/Media/SDLMedia/SdlInput.h>
#include <XChip/Media/SDLMedia/SdlSound.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/listbox.h>


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
	bool running, closing;
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
	
	wxDECLARE_EVENT_TABLE();
    
};

#endif
