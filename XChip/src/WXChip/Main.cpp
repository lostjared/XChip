/*

WXChip - chip8 emulator using XChip library and a wxWidgets gui.
Copyright (C) 2016  Jared Bruni, Rafael Moura.


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/msgdlg.h>

#include <string>
#include <Utix/Memory.h>
#include <WXChip/MainWindow.h>
#include <WXChip/Main.h>

wxIMPLEMENT_APP(WXChip);


bool WXChip::OnInit()
{
	using utix::make_unique;
	try {
		if( wxApp::OnInit() ) 
		{
			auto mainwin = make_unique<MainWindow>("WXChip", wxPoint(50,50), wxSize(800, 600));
			mainwin->Show(true);
			SetTopWindow(mainwin.release());
			return true;
		}
	}
	catch(std::exception& err) {
		wxMessageBox(err.what(), wxT("Fatal Exception"), wxICON_ERROR);
		// continues and returns false
	}
	return false;
}


int WXChip::OnExit() 
{
	return wxApp::OnExit();
}
