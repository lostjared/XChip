#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/dirdlg.h>
#include <Utix/RWrap.h>
#include <WXChip/Dialog.h>



utix::RWrap<DIR*, void(*)(DIR*)> DirDlg(wxFrame* parent, const wxString& msg, wxString* outPath)
{

	wxDirDialog dirDlg(parent, msg, wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if(dirDlg.ShowModal() == wxID_OK)
	{
		wxString path = dirDlg.GetPath();
		if(path != "nopath")
		{
			DIR* dir;
			errno = 0;
			if( ( dir = opendir(path.c_str()) ) != nullptr) 
			{
				if( outPath )
					*outPath = std::move(path);

				const auto cleanup = [](DIR* d) { closedir(d); }; 
				return utix::make_rwrap( (DIR*) dir, (void(*)(DIR*)) cleanup);
			}
			else 
			{
				const int errno_code = errno;
				const wxString errMsg = 
                                               "Couldn't open directory \"" + path + "\". " + strerror(errno_code);

				WarningBox(parent, errMsg);
			}
		}
	}

	return utix::make_rwrap( (DIR*)nullptr, ( void(*)(DIR*) ) nullptr);
}











