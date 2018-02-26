#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#include "jls/app.h"
#include "jls/appframe.h"

wxIMPLEMENT_APP(jlsApp);

bool jlsApp::OnInit()
{
	jlsAppFrame *frame = new jlsAppFrame();
	frame->Show(true);
	return true;
}

/*
* TODO
*
* - Remove extra echo from process interaction.
* - Figure out why julia behaves differently (stderr?).
* - Configuration and settings (+storage).
* - Think about plotting.
* - Look through jlsProcess and clean it up a bit.
*
* - Implement async process i/o for linux and macOS?
*/
