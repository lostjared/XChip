#ifndef WXCHIP_MESSAGEBOX_H_
#define WXCHIP_MESSAGEBOX_H_
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/frame.h>


inline bool YesNoBox(const wxString& msg, wxFrame* frame) {
	return wxMessageBox(msg, wxT("WXChip - Confirm"), wxYES_NO | wxCANCEL, frame) == wxYES;
}

inline void FatalExceptionBox(const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Fatal Exception"), wxICON_ERROR);
}

inline void WarningBox(const wxString& msg, wxFrame* frame) {
	wxMessageBox(msg, wxT("WXChip - Warning"), wxICON_WARNING, frame);
}



















#endif






