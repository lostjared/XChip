#ifndef _WXCHIP_MAINWINDOW_H_
#define _WXCHIP_MAINWINDOW_H_

#include <XChip/Utility/Memory.h>
#include <wXChip/SettingsWindow.h>
#include <wXChip/GLWindow.h>

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
private:
	
	
	void OnChip(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnLDown(wxMouseEvent &event);
	void OnMouseOver(wxMouseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnWindowClose(wxCloseEvent &event);
	void OnStartRom(wxCommandEvent &event);
	void LoadSettings(wxCommandEvent &event);
	void LaunchRom();

	std::unique_ptr<wxPanel> _panel;
	std::unique_ptr<wxStaticText> _text;

	std::unique_ptr<wxListBox> _listBox;
	std::unique_ptr<wxButton> _startRom;
	std::unique_ptr<wxButton> _settings;
	std::unique_ptr<wxButton> _emulatorSettings;
	std::unique_ptr<SettingsWindow> _settingsWin;
	std::unique_ptr<GLWindow> _glWin;
	
	std::string _filePath;

	wxDECLARE_EVENT_TABLE();
    
};








#endif