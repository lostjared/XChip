#ifndef WXCHIP_DIALOG_H_
#define WXCHIP_DIALOG_H_

#ifdef _WIN32
#include <cstdlib>
#include <WXChip/dirent.h>
#elif defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#endif

#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/colour.h>


inline bool ConfirmDlg(wxFrame* const parent, const wxString& msg) {
	return wxMessageBox(msg, wxT("WXChip - Confirm"), wxYES_NO | wxCANCEL, parent) == wxYES;
}


inline void FatalExceptionDlg(const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Fatal Exception"), wxICON_ERROR);
}


inline void WarningDlg(wxFrame* const parent, const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Warning"), wxICON_WARNING, parent);
}


inline void ErrorDlg(wxFrame* const parent, const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Error"), wxICON_ERROR, parent);	
}


inline void InformationDlg(wxFrame* const parent, const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Information"), wxOK | wxICON_INFORMATION, parent);
}


// returns the directory path if succeed , empty string if fails
wxString DirectoryDlg(wxFrame* const parent, const wxString& msg);
// returns the file path if succeed , empty string if fails
wxString FileDlg(wxFrame* const parent, const wxString& msg, const wxString& wildcard);
// returns true if color is modified, false if not
bool ColourDlg(wxFrame* const parent, wxColour& color);















#endif // WXCHIP_DIALOG_H_






