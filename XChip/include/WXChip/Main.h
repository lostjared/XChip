#ifndef _WXCHIP_MAIN_H_
#define _WXCHIP_MAIN_H_

#include <wx/app.h>

class MainWindow;

class WXChip final : public wxApp
{
public:
	bool OnInit() override;
	int OnExit() override;
	int FilterEvent(wxEvent& event) override;

private:
	MainWindow* _mainwin;

};














































#endif










































































