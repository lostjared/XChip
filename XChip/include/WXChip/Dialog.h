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
#include <Utix/RWrap.h>


inline bool ConfirmDlg(wxFrame* parent, const wxString& msg) {
	return wxMessageBox(msg, wxT("WXChip - Confirm"), wxYES_NO | wxCANCEL, parent) == wxYES;
}

inline void FatalExceptionDlg(const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Fatal Exception"), wxICON_ERROR);
}

inline void WarningDlg(wxFrame* parent, const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Warning"), wxICON_WARNING, parent);
}
inline void InformationDlg(wxFrame* parent, const wxString& msg) {
	wxMessageBox(msg, wxT("WXChip - Information"), wxOK | wxICON_INFORMATION);
}

// get a RWrap with a dir ptr which will be cleaned in scope's end
extern utix::RWrap<DIR*, void(*)(DIR*)> 
DirectoryDlg(wxFrame* parent, const wxString& msg, wxString* outPath = nullptr);

















#endif // WXCHIP_DIALOG_H_






