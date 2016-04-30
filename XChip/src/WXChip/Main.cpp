#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <XChip/Utility/Memory.h>

#include <WXChip/Main.h>
#include <WXChip/MainWindow.h>




wxIMPLEMENT_APP(WXChip);


bool WXChip::OnInit()
{
	using xchip::utility::make_unique;

	try {
		auto mainwin = make_unique<MainWindow>("WXChip", wxPoint(50,50), wxSize(800, 600));
		mainwin->Show(true);
		mainwin->SetFocus();
		_mainwin = mainwin.release();

	}
	catch(std::exception& err) {
		std::cout << err.what() << std::endl;
		return false;
	}


	return true;
}



int WXChip::OnExit()
{
	return 0;
}



