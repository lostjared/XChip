#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/colordlg.h>
#include <wx/colourdata.h>
#include <WXChip/Dialog.h>



wxString DirectoryDlg(wxFrame* const parent, const wxString& msg)
{

	wxDirDialog dirDlg(parent, msg, wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if(dirDlg.ShowModal() == wxID_OK)
		return dirDlg.GetPath();

	return wxString();
}




wxString FileDlg(wxFrame* const parent, const wxString& msg, const wxString& wildcard) 
{
	wxFileDialog fdlg(parent, msg, wxEmptyString, wxEmptyString, wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fdlg.ShowModal() == wxID_OK)
		return fdlg.GetPath();

	return wxString();
}




bool ColourDlg(wxFrame* const parent, wxColour& color)
{
	wxColourData data;
	data.SetColour(color);
	wxColourDialog dialog(parent, &data);

	if (dialog.ShowModal() == wxID_OK) {
		color = dialog.GetColourData().GetColour();
		return true;
	}

	return false;	
}





